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
        dcp_name, set_name = base_name.split('_Requests_')
        set_name = set_name.lower()

        # Read the CSV and convert to tuples
        with open(file_path, mode='r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            data_tuples = []
            for row in reader:
                source = int(row['Src_node'])
                destination = int(row['Dst_node'])
                priority = int(row['Final_Priority'])
                max_bw = int(row['Max_Required_Lightpath_Bandwidth'])/1000
                min_bw = int(row['Min_Required_Lightpath_Bandwidth'])/1000
                degradation_tolerance = round((max_bw - min_bw) / max_bw, 2)
                carrier = f"carrier{int(row['Carrier_ID']) + 1}"
                users_count = int(row['Users_count'])

                item = (source, destination, priority, max_bw, degradation_tolerance, carrier, users_count)
                data_tuples.append(item)
                requests.append(item)

        # Insert into the dictionary
        if dcp_name not in dcp_sets:
            dcp_sets[dcp_name] = {}
        dcp_sets[dcp_name][set_name] = data_tuples

    return dcp_sets, requests

#Subhadeep CHECK input file folder
input_dcp = (
    'DCP1_Requests_1.csv',
    'DCP1_Requests_2.csv',
    'DCP2_Requests_1.csv',
    'DCP2_Requests_2.csv'
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
            capacities[edge] = 400
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
    
#Subhadeep CHECK input file folder
nodes_A,edges_A,link_status_A,capacities_A,prices_A, normal_prices_A, nodes_B,edges_B,link_status_B,capacities_B,prices_B, normal_prices_B = load_carrier_from_csv(
#    "/home/dellirio/Scaricati/Ignazio_ECOC_2025_0408/Price_generation/Carrier_0_links.csv",
#    "/home/dellirio/Scaricati/Ignazio_ECOC_2025_0408/Price_generation/Carrier_1_links.csv"
    "Carrier_0_links.csv",
    "Carrier_1_links.csv"
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
    
    # Print debug information about nodes in each carrier's network
    print("\nCarrier Network Information:")
    for carrier in carriers:
        print(f"\n{carrier} nodes: {sorted(nodes[carrier])}")
        print(f"{carrier} edges: {sorted(edges[carrier])}")
    
    # Create a unified set of bidirectional edges for each carrier
    bidirectional_edges = {a: set() for a in carriers}
    for a in carriers:
        for u, v in edges[a]:
            bidirectional_edges[a].add((u, v))
            bidirectional_edges[a].add((v, u))

    # Print debug information about requests
    print("\nRequest Information:")
    for request in requests:
        source, destination, priority, required_bandwidth, degradation_tolerance, carrier_id, users_count = request
        print(f"Request: {source}->{destination} on {carrier_id}")
    
    # Convert to a dictionary with capacities for each carrier
    edge_capacities = {a: {} for a in carriers}
    # Add dictionary to track used capacity on each edge
    edge_used_capacity = {a: {} for a in carriers}
    for a in carriers:
        for u, v in bidirectional_edges[a]:
            edge_capacities[a][(u, v)] = carrier_capacities[a].get((u, v), carrier_capacities[a].get((v, u), 0))
            edge_used_capacity[a][(u, v)] = 0  # Initialize used capacity to 0
            

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
    ksp_groups_valid = True  # Changed meaning: at least one request can be solved

    # To store KSP results
    ksp_total_cost = 0
    ksp_total_normal_cost = 0
    ksp_total_bandwidth = 0
    ksp_num_failed_links = 0
    ksp_priority = 0  # Sum of priorities
    ksp_info = {}

    # Iterate over each DCP group
    for dcp_name, groups in dcp_groups.items():
        # Dictionary to hold the overall costs and paths for each group
        group_costs = {}
        group_solved_requests = {}  # Track number of solved requests per group
        ksp_info[dcp_name] = {}

        # Evaluate each group within the current DCP group
        for group_name, ksp_requests in groups.items():
            # Store total cost, minimum bandwidth, and failed links count for the group
            total_cost = 0
            total_normal_cost = 0
            total_failed_links = 0
            total_bandwidth = 0
            total_priority = 0
            solved_count = 0  # Count of successfully solved requests
            solved_requests = {}  # Track which requests were solved

            # Create temporary capacity tracking for this group
            temp_edge_used_capacity = {a: {edge: used for edge, used in edge_used_capacity[a].items()} for a in carriers}

            # Iterate over each request in the group
            for request in ksp_requests:
                source, destination, priority, required_bandwidth, degradation_tolerance, carrier_id, users_count = request
                request_key = f"{source}->{destination}"

                # Build the graph for the current carrier
                G = nx.DiGraph()
                carrier_edges = edge_capacities[carrier_id].keys()
                carrier_nodes = nodes[carrier_id]

                for (u, v) in carrier_edges:
                    capacity = edge_capacities[carrier_id][(u, v)]
                    used_capacity = temp_edge_used_capacity[carrier_id][(u, v)]
                    remaining_capacity = capacity - used_capacity
                    price = edge_prices[carrier_id][(u, v)]
                    normal_price = edge_normal_prices[carrier_id][(u, v)]
                    failed = link_status[carrier_id][(u, v)]
                    
                    # Calculate capacity-aware weight
                    if remaining_capacity > 0:
                        # Add a capacity factor to the weight
                        capacity_factor = 1 + (used_capacity / capacity)  # Will be between 1 and 2
                        edge_weight = price * capacity_factor
                    else:
                        # If no capacity remains, assign a very high weight
                        edge_weight = float('inf')
                    
                    G.add_edge(u, v, weight=edge_weight, normal_price=normal_price, 
                              capacity=remaining_capacity, failed=failed, 
                              base_price=price, remaining=remaining_capacity)

                # Check if source and destination exist in the carrier's network
                if source not in carrier_nodes or destination not in carrier_nodes:
                    # Store unsolved request - nodes don't exist in carrier's network
                    solved_requests[request_key] = {
                        'request': request,
                        'path': [],
                        'failed_links': [],
                        'cost': 0,
                        'normal_cost': 0,
                        'bandwidth': required_bandwidth,
                        'result': 0  # Request could not be solved
                    }
                    continue
                else:
                    try:
                        paths = list(nx.shortest_simple_paths(G, source, destination, weight='weight'))[:K]
                        valid_paths = []

                        for path in paths:
                            path_edges = list(zip(path[:-1], path[1:]))
                            # Check if path has sufficient capacity
                            min_capacity = float('inf')
                            for u, v in path_edges:
                                edge_data = G.edges[u, v]
                                min_capacity = min(min_capacity, edge_data['remaining'])
                            
                            if min_capacity >= required_bandwidth:
                                cost = sum(float(G.edges[u, v]['base_price']) for u, v in path_edges)
                                normal_cost = sum(float(edge_normal_prices[carrier_id].get((u, v), 0)) for u, v in path_edges)
                                failed_links = [(u, v) for u, v in path_edges if link_status[carrier_id][(u ,v)]== 1]
                                failed_count = len(failed_links)

                                valid_paths.append({
                                    'path': path,
                                    'cost': float(cost),
                                    'normal_cost': float(normal_cost),
                                    'min_bandwidth': float(required_bandwidth),
                                    'failed_links': failed_links,
                                    'failed_count': float(failed_count),
                                    'priority': priority,
                                    'path_edges': path_edges
                                })
                            
                        if valid_paths and any(valid_paths):
                            best_path = min(valid_paths, key=lambda x: x['cost'])  # Path with minimum cost
                            solved_requests[request_key] = {
                                'request': request,
                                'path': best_path['path'],
                                'failed_links': best_path['failed_links'],
                                'cost': best_path['cost'],
                                'normal_cost': best_path['normal_cost'],
                                'bandwidth': required_bandwidth,
                                'result': 1  # Request solved successfully
                            }
                            
                            # Update group statistics
                            total_cost += best_path['cost']
                            total_normal_cost += best_path['normal_cost']
                            total_failed_links += best_path['failed_count']
                            total_bandwidth += best_path['min_bandwidth']
                            total_priority += best_path['priority']
                            solved_count += 1

                            # Update temporary used capacity for this path
                            for u, v in best_path['path_edges']:
                                temp_edge_used_capacity[carrier_id][(u, v)] += required_bandwidth
                                temp_edge_used_capacity[carrier_id][(v, u)] += required_bandwidth  # Update reverse direction too
                        else:
                            # Store unsolved request
                            solved_requests[request_key] = {
                                'request': request,
                                'path': [],
                                'failed_links': [],
                                'cost': 0,
                                'normal_cost': 0,
                                'bandwidth': required_bandwidth,
                                'result': 0  # Request could not be solved
                            }
                        
                    except nx.NetworkXNoPath:
                        # Store unsolved request
                        solved_requests[request_key] = {
                            'request': request,
                            'path': [],
                            'failed_links': [],
                            'cost': 0,
                            'normal_cost': 0,
                            'bandwidth': required_bandwidth,
                            'result': 0  # Request could not be solved
                        }

            # Store group results if at least one request was solved
            if solved_count > 0:
                group_costs[group_name] = (total_cost, total_normal_cost, total_failed_links, total_bandwidth, total_priority)
                group_solved_requests[group_name] = (solved_count, solved_requests, temp_edge_used_capacity)
        
        # Select the group with the most solved requests, breaking ties by total cost
        if group_solved_requests:
            selected_group = max(group_solved_requests.items(), 
                               key=lambda x: (x[1][0], -group_costs[x[0]][0]))[0]
            
            solved_count, solved_requests, temp_edge_used_capacity = group_solved_requests[selected_group]
            total_cost, total_normal_cost, total_failed_links, total_bandwidth, total_priority = group_costs[selected_group]
            
            print(f"Selected Group: {int(selected_group[-1])}  " 
                  f"Solved Requests: {solved_count}/{len(ksp_requests)}, "
                  f"Total Cost: {total_cost}, Total Normal Cost: {total_normal_cost}, "
                  f"Failed Links: {total_failed_links}, Total Bandwidth: {total_bandwidth}, "
                  f"Sum of Priority: {total_priority}")

            # Store the solved and unsolved requests information
            ksp_info[dcp_name] = solved_requests
            
            # Update the actual used capacity with the values from selected group
            edge_used_capacity = temp_edge_used_capacity

            # Print capacity usage information for the selected group
            print("\nCapacity Usage Summary:")
            for carrier in carriers:
                print(f"\n{carrier} links:")
                for (u, v) in bidirectional_edges[carrier]:
                    used = edge_used_capacity[carrier][(u, v)]
                    total = edge_capacities[carrier][(u, v)]
                    if used > 0:  # Only show links that are being used
                        print(f"  Link {u}->{v}: Used {used}/{total} ({(used/total)*100:.1f}%)")

            # Accumulate KSP results
            ksp_total_cost += total_cost
            ksp_total_normal_cost += total_normal_cost
            ksp_total_bandwidth += total_bandwidth
            ksp_num_failed_links += total_failed_links
            ksp_priority += total_priority
        else:
            print("  No requests could be solved in any group")
            ksp_groups_valid = False

    # Create CSV with detailed request information for KSP algorithm
    carrier1_data = []
    carrier2_data = []
    request1_counter = 1  # Initialize request counter for carrier 1
    request2_counter = 1  # Initialize request counter for carrier 2
    carrier1_lead_time = 0
    carrier2_lead_time = 0
    request_to_dcp = {}
    carrier_failed_links = {
        'carrier1': {},
        'carrier2': {}
    }

    # Prepare data for CSV
    for dcp_name in ksp_info:
        for request_key, request_info in ksp_info[dcp_name].items():
            source, destination = map(int, request_key.split('->'))
            original_request = request_info['request']
            
            source, destination, priority, bandwidth, degradation_tolerance, carrier_id, users_count = original_request
            min_bw = int(bandwidth * (1 - degradation_tolerance))
            max_bw = bandwidth
            
            # Calculate path information
            path = request_info['path']
            result = request_info['result']
            current_lead_time = 0
            
            if result == 1:  # Request was solved
                path_edges = list(zip(path[:-1], path[1:]))
                failed_links = request_info['failed_links']
                failed_links_str = ', '.join([f"{u}->{v}" for u, v in failed_links])
                path_str = '->'.join(map(str, path))
                failed_links_count = len(failed_links)
                successful_links_count = len(path_edges) - failed_links_count
                total_hops = len(path_edges)
                total_cost = request_info['cost']
                total_normal_cost = request_info['normal_cost']
                
                # Calculate lead time for failed links
                for u, v in failed_links:
                    sorted_link = tuple(sorted([u, v]))
                    if sorted_link not in carrier_failed_links[carrier_id]:
                        if carrier_id == 'carrier1':
                            carrier1_lead_time += 1
                            carrier_failed_links[carrier_id][sorted_link] = carrier1_lead_time
                        elif carrier_id == 'carrier2':
                            carrier2_lead_time += 1
                            carrier_failed_links[carrier_id][sorted_link] = carrier2_lead_time
                    link_lead_time = carrier_failed_links[carrier_id][sorted_link]
                    current_lead_time = max(current_lead_time, link_lead_time)
            else:  # Request was not solved
                failed_links_str = ''
                path_str = ''
                failed_links_count = 0
                successful_links_count = 0
                total_hops = 0
                total_cost = 0
                total_normal_cost = 0
                current_lead_time = 0

            # Create row for CSV
            row = {
                'ID': request1_counter if carrier_id == 'carrier1' else request2_counter,
                'Src_node': source,
                'Dst_node': destination,
                'Min_Req_BW': int(min_bw),
                'Max_Req_BW': int(max_bw),
                'Satisfied_Bandwidth': int(max_bw) if result == 1 or result == 2 else 0,
                'Latency': 20,  # Constant value
                'Result': 2 if current_lead_time > 0 else (1 if result == 1 else 0),  # Set to 2 if there's lead time
                'Lead_Time': current_lead_time,
                'Score': 100 if result == 1 or result == 2 else 0,
                'Customer_ID': int('3') if dcp_name == 'DCP1' else int('4'),
                'Priority': priority,
                'Users_count': users_count,
                'Price': total_cost,
                'Normal_Price': total_normal_cost,
                'Used_F_Links': failed_links_str,
                'Route': path_str,
                'F_Links': failed_links_count,
                'S_Links': successful_links_count,
                'Hops': total_hops
            }

            if carrier_id == 'carrier1':
                carrier1_data.append(row)
                request1_counter += 1
            else:
                carrier2_data.append(row)
                request2_counter += 1

    # CSV fields definition
    csv_fields = ['ID', 'Src_node', 'Dst_node', 'Min_Req_BW', 'Max_Req_BW', 
                 'Satisfied_Bandwidth', 'Latency', 'Result', 'Lead_Time', 'Score',
                 'Customer_ID', 'Priority', 'Users_count', 'Price', 'Normal_Price', 'Used_F_Links', 'Route',
                 'F_Links','S_Links','Hops']
    
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



