from docplex.mp.model import Model
import networkx as nx
import csv
import os


def load_dcp_sets_from_csv(file_paths):
    dcp_sets = {}
    requests = []

    for file_path in file_paths:
        # Extract DCP and set names from file name, e.g., "DCP1_Set1.csv"
        base_name = os.path.basename(file_path).replace('.csv', '')
        dcp_name, set_name = base_name.split('_')
        set_name = set_name.lower()

        # Read the CSV and convert to tuples
        with open(file_path, mode='r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            data_tuples = []
            for row in reader:
                source = int(row['Src_node'])
                destination = int(row['Dst_node'])
                priority = int(row['Final_Priority'])
                max_bw = int(row['Max_Required_Lightpath_Bandwidth'])
                min_bw = int(row['Min_Required_Lightpath_Bandwidth'])
                degradation_tolerance = round((max_bw - min_bw) / max_bw, 2)
                carrier = f"carrier{int(row['Carrier_ID']) + 1}"

                item = (source, destination, priority, max_bw, degradation_tolerance, carrier)
                data_tuples.append(item)
                requests.append(item)

        # Insert into the dictionary
        if dcp_name not in dcp_sets:
            dcp_sets[dcp_name] = {}
        dcp_sets[dcp_name][set_name] = data_tuples

    return dcp_sets, requests

#Subhadeep CHECK input file folder
input_dcp = (
    '/home/dellirio/Scaricati/Ignazio_ECOC_2025_0408/DCP1_Set_Creation/DCP1_Set1.csv',
    '/home/dellirio/Scaricati/Ignazio_ECOC_2025_0408/DCP1_Set_Creation/DCP1_Set2.csv',
    '/home/dellirio/Scaricati/Ignazio_ECOC_2025_0408/DCP2_Set_Creation/DCP2_Set1.csv',
    '/home/dellirio/Scaricati/Ignazio_ECOC_2025_0408/DCP2_Set_Creation/DCP2_Set2.csv'
)
dcp_groups, requests = load_dcp_sets_from_csv(input_dcp)


def load_carrier_from_csv(carrier_0_path, carrier_1_path):
    def read_csv(filepath):
        with open(filepath, newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            return [row for row in reader]

    def extract_data(carrier_data, carrier_id):
        nodes = set()
        edges = []
        link_status = {}
        capacities = {}
        prices = {}
        normal_prices = {}

        for row in carrier_data:
            src = int(row['Src_node'])
            dst = int(row['Dst_node'])
            status = int(row['Flag'])

            edge = (src, dst)
            nodes.update([src, dst])
            edges.append(edge)
            link_status[edge] = int(not status)  # invert link status
            capacities[edge] = 400000
            #Set price based on flag value
            price = 52  if status == 0  else 2
            prices[edge] = price
            prices[(dst, src)] = price
            normal_prices[edge] = 2
            normal_prices[(dst, src)] = 2
        return sorted(nodes), edges, link_status, capacities, prices, normal_prices

    # Read the CSV files
    carrier_0_data = read_csv(carrier_0_path)
    carrier_1_data = read_csv(carrier_1_path)

    # Extract structured data with carrier filtering
    nodes_A, edges_A, link_status_A, capacities_A, prices_A, normal_prices_A = extract_data(carrier_0_data, carrier_id=0)
    nodes_B, edges_B, link_status_B, capacities_B, prices_B, normal_prices_B = extract_data(carrier_1_data, carrier_id=1)

    return nodes_A,edges_A,link_status_A,capacities_A,prices_A,normal_prices_A, nodes_B,edges_B,link_status_B,capacities_B,prices_B, normal_prices_B
    
#Subhadeep CHECK output file folder
nodes_A,edges_A,link_status_A,capacities_A,prices_A, normal_prices_A, nodes_B,edges_B,link_status_B,capacities_B,prices_B, normal_prices_B = load_carrier_from_csv(
    "/home/dellirio/Scaricati/Ignazio_ECOC_2025_0408/Price_generation/Carrier_0_links.csv",
    "/home/dellirio/Scaricati/Ignazio_ECOC_2025_0408/Price_generation/Carrier_1_links.csv"
)



time_slots = range(20)

# Define the two carriers
carriers = ['carrier1', 'carrier2']

    






def create_ksp_model(nodes_A, nodes_B, edges_A, edges_B, requests, capacities_A, 
                     capacities_B, prices_A, prices_B, normal_prices_A, normal_prices_B,
                     link_status_A, link_status_B, time_slots, carriers, 
                     weight1=1000000, weight2=0.01, weight3=100, weight4=1000, weight5=0.001):
    
    carriers = ['carrier1', 'carrier2']
    nodes = {'carrier1': nodes_A, 'carrier2': nodes_B}
    edges = {'carrier1': edges_A, 'carrier2': edges_B}
    carrier_capacities = {'carrier1': capacities_A, 'carrier2': capacities_B}
    carrier_prices = {'carrier1': prices_A, 'carrier2': prices_B}
    carrier_normal_prices = {'carrier1': normal_prices_A, 'carrier2': normal_prices_B}
    carrier_link_status = {'carrier1': link_status_A, 'carrier2': link_status_B}
    
    # Create a unified set of bidirectional edges for each carrier
    bidirectional_edges = {a: set() for a in carriers}
    for a in carriers:
        for u, v in edges[a]:
            bidirectional_edges[a].add((u, v))
            bidirectional_edges[a].add((v, u))


    # Convert to a dictionary with capacities for each carrier
    edge_capacities = {a: {} for a in carriers}
    for a in carriers:
        for u, v in bidirectional_edges[a]:
            edge_capacities[a][(u, v)] = carrier_capacities[a].get((u, v), carrier_capacities[a].get((v, u), 0))
            

    # Prices over bidirectional edges for each carrier
    edge_prices = {a: {} for a in carriers}
    edge_normal_prices = {a: {} for a in carriers}
    for a in carriers:
        for u, v in bidirectional_edges[a]:
            edge_prices[a][(u, v)] = carrier_prices[a].get((u, v), carrier_prices[a].get((v, u), 0))
            edge_prices[a][(v, u)] = edge_prices[a][(u, v)]
            edge_normal_prices[a][(u, v)] = carrier_normal_prices[a].get((u, v), carrier_normal_prices[a].get((v, u), 0))
            edge_normal_prices[a][(v, u)] = edge_normal_prices[a][(u, v)]

    # Link status over bidirectional edges for each carrier
    link_status = {a: {} for a in carriers}
    for a in carriers:
        for u, v in bidirectional_edges[a]:
            link_status[a][(u, v)] = carrier_link_status[a].get((u, v), carrier_link_status[a].get((v, u), 0))
            link_status[a][(v, u)] = link_status[a][(u, v)]
    


    # Number of shortest paths to evaluate for each request
    K = 3
    ksp_groups_valid = True #to ensure every group has a valid solution    

    # To store KSP results
    ksp_total_cost = 0
    ksp_total_normal_cost = 0
    ksp_total_bandwidth = 0
    ksp_num_failed_links = 0
    ksp_priority = 0  # Sum of priorities
    ksp_info = {}

    # Iterate over each DCP group
    for dcp_name, groups in dcp_groups.items():
        #print(f"DCP Group: {dcp_name}")
        # Dictionary to hold the overall costs and paths for each group
        group_costs = {}
        ksp_info[dcp_name] = {}

        # Evaluate each group within the current DCP group
        for group_name, ksp_requests in groups.items():
            #print(f" Analyzing Group: {group_name}")

            # Store total cost, minimum bandwidth, and failed links count for the group
            total_cost = 0
            total_normal_cost = 0
            total_failed_links = 0
            total_bandwidth = 0
            total_priority = 0
            valid = True  # Flag to check if all requests in the group have valid paths

            # Iterate over each request in the group
            for request in ksp_requests:
                source, destination, priority, required_bandwidth, degradation_tolerance, carrier_id = request
                #print(f" Analyzing request: {source} -> {destination}")

                # Build the graph for the current carrier
                G = nx.DiGraph()
                carrier_edges = edge_capacities[carrier_id].keys() #new

                for (u, v) in carrier_edges:
                    capacity = edge_capacities[carrier_id][(u, v)] #new
                    price = edge_prices[carrier_id][(u, v)] #new
                    normal_price = edge_normal_prices[carrier_id][(u, v)] #new
                    failed = link_status[carrier_id][(u, v)] 
                    G.add_edge(u, v, weight=price, normal_price =normal_price, capacity =capacity, failed=failed) 
                            


                try:
                    paths = list(nx.shortest_simple_paths(G, source, destination, weight='weight'))[:K]
                    valid_paths = []
                    best_path = []

                    for path in paths:
                        path_edges = list(zip(path[:-1], path[1:]))
                        cost = sum(float(edge_prices[carrier_id].get((u, v), 0)) for u, v in path_edges)
                        normal_cost = sum(float(edge_normal_prices[carrier_id].get((u, v), 0)) for u, v in path_edges)
                        capacities = [float(edge_capacities[carrier_id][(u, v)]) for u, v in path_edges]
                        min_capacity = min(capacities)
                        failed_links = [(u, v) for u, v in path_edges if link_status[carrier_id][(u ,v)]== 1]
                        failed_count = len(failed_links)

                        if min_capacity >= required_bandwidth:
                            #print (f"       Path accepted, sufficient capacity {required_bandwidth} needed {min_capacity} offered, cost {cost}, failed link count: {failed_links}{failed_count}")
                            valid_paths.append({
                                'path': path,
                                'cost': float(cost),
                                'normal_cost': float(normal_cost),
                                'min_bandwidth' : float(required_bandwidth),
                                'failed_links': failed_links,
                                'failed_count': float(failed_count),
                                'priority': priority
                            })
                        
                    if valid_paths and any(valid_paths):
                        best_path = min(valid_paths, key=lambda x: x['cost'])  # Path with minimum cost
                        request_key =f"{source}->{destination}"
                        ksp_info[dcp_name][request_key] = {
                            'path': best_path['path'],
                            'failed_links': best_path['failed_links']
                        }
                        # Update group statistics
                        total_cost += best_path.get('cost')
                        total_normal_cost += best_path.get('normal_cost')
                        #print(f"        best path {best_path}")
                        #print(f"        total cost {total_cost}")
                        total_failed_links += best_path.get('failed_count')
                        total_bandwidth += best_path.get('min_bandwidth')
                        total_priority += best_path.get('priority')

                    else:
                        #print(f"    path rejected insufficient capacity {required_bandwidth} needed {min_capacity} offered")
                        valid= False                
                      
                except nx.NetworkXNoPath:
                    valid = False
                    #print(f"  No path found between {source} -> {destination}.")
                    valid_paths.append([])



            #If all requests in the group have valid paths, store the results
            if valid:
                group_costs[group_name] = (total_cost, total_normal_cost, total_failed_links, total_bandwidth, total_priority)
                #print(f"RESULTS {group_costs}")

            else:
                #print(f"    No valid path for group {group_name}.")
                continue
        
        # Select the group with the overall lowest total cost
        if ksp_groups_valid and group_costs:
            selected_group, (total_cost, total_normal_cost, total_failed_links, total_bandwidth, total_priority) = min(
                group_costs.items(), key=lambda x: x[1][0])
            print(f"Selected Group: {int(selected_group[-1])}  " 
                f"Total Cost: {total_cost}, Total Normal Cost{total_normal_cost} Failed Links: {total_failed_links}, "
                f"Total Bandwidth: {total_bandwidth}, Sum of Priority: {total_priority}")

            

            # Accumulate KSP results
            ksp_total_cost += total_cost
            ksp_total_normal_cost += total_normal_cost
            ksp_total_bandwidth += total_bandwidth
            ksp_num_failed_links += total_failed_links
            ksp_priority += total_priority
        else:
            print("  No valid groups found with all requests satisfied. No KSP results")
            ksp_groups_valid= False
        


    # Create CSV with detailed request information for KSP algorithm
    ksp_results_data = []
    request1_counter = 1  # Initialize request counter for carrier 1
    request2_counter = 1  # Initialize request counter for carrier 2
    carrier1_lead_time = 0
    carrier2_lead_time = 0
    carrier_failed_links = {
        'carrier1': {},
        'carrier2': {}
    }

    # Prepare data for CSV
    for dcp_name in ksp_info:
        for request_key, path_info in ksp_info[dcp_name].items():
            source, destination = map(int, request_key.split('->'))
            
            # Find the original request details
            original_request = None
            for req in dcp_groups[dcp_name][selected_group]:
                if req[0] == source and req[1] == destination:
                    original_request = req
                    break
            
            if original_request:
                source, destination, priority, bandwidth, degradation_tolerance, carrier_id = original_request
                min_bw = int(bandwidth * (1 - degradation_tolerance))
                max_bw = bandwidth
                
                # Calculate path cost
                path = path_info['path']
                path_edges = list(zip(path[:-1], path[1:]))
                total_cost = sum(float(edge_prices[carrier_id].get((u, v), 0)) for u, v in path_edges)
                total_normal_cost = sum(float(edge_normal_prices[carrier_id].get((u, v), 0)) for u, v in path_edges)

                if carrier_id == 'carrier1':
                    request_counter = request1_counter
                    request1_counter += 1
                elif carrier_id == 'carrier2':
                    request_counter = request2_counter
                    request2_counter += 1

                failed_links = []
                current_lead_time = 0
                result = 1
                for u, v in path_edges:
                    if link_status[carrier_id][(u, v)] == 1:
                        sorted_link = tuple(sorted([u, v]))
                        failed_links.append((u, v))
                        result = 2
                        if sorted_link not in carrier_failed_links[carrier_id]:
                            if carrier_id == 'carrier1':
                                carrier1_lead_time += 1
                                carrier_failed_links[carrier_id][sorted_link] = carrier1_lead_time
                            elif carrier_id == 'carrier2':  
                                carrier2_lead_time += 1
                                carrier_failed_links[carrier_id][sorted_link] = carrier2_lead_time
                        link_lead_time = carrier_failed_links[carrier_id][sorted_link]
                        current_lead_time = max(current_lead_time, link_lead_time)
                    
                # Format failed links and path for CSV
                failed_links_str = ', '.join([f"{u}->{v}" for u, v in path_info['failed_links']])
                path_str = '->'.join(map(str, path_info['path']))
                
                # Create row for CSV
                row = {
                    'ID': request_counter,
                    'Src_node': source,
                    'Dst_node': destination,
                    'Min_Req_BW': min_bw,
                    'Max_Req_BW': max_bw,
                    'Satisfied_Bandwidth': max_bw,  # Using max bandwidth as satisfied bandwidth
                    'Latency': 20,  # Constant value
                    'Result': result,
                    'Lead_Time': current_lead_time,  
                    'Score': 100,  # Constant value
                    'Customer_ID': int('3') if carrier_id == 'carrier1' else '4',
                    'Priority': priority,
                    'Price': total_cost,
                    'Normal_Price': total_normal_cost,
                    'Used_F_Links': failed_links_str,
                    'Route': path_str
                }
                ksp_results_data.append(row)


    # Split data by carrier
    carrier1_data = []
    carrier2_data = []
    
    for row in ksp_results_data:
        if row['Customer_ID'] == 3:
            carrier1_data.append(row)
        else: 
            carrier2_data.append(row)
    
    # CSV fields definition
    csv_fields = ['ID', 'Src_node', 'Dst_node', 'Min_Req_BW', 'Max_Req_BW', 
                 'Satisfied_Bandwidth', 'Latency', 'Result', 'Lead_Time', 'Score',
                 'Customer_ID', 'Priority', 'Price', 'Normal_Price', 'Used_F_Links', 'Route']
    
    #Subhadeep CHECK output file folder
    # Write carrier1 data to CSV file
    csv_filename1 = "ksp_carrier0_Final.csv"
    with open(csv_filename1, mode='w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=csv_fields)
        writer.writeheader()
        writer.writerows(carrier1_data)
    
    # Write carrier2 data to CSV file
    csv_filename2 = "ksp_carrier1_Final.csv"
    with open(csv_filename2, mode='w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=csv_fields)
        writer.writeheader()
        writer.writerows(carrier2_data)
    
    print(f"Results have been written to {csv_filename1} and {csv_filename2}")


create_ksp_model(nodes_A, nodes_B, edges_A, edges_B, requests, capacities_A, capacities_B, prices_A, prices_B, 
                        normal_prices_A, normal_prices_B, link_status_A, link_status_B, time_slots, carriers)



