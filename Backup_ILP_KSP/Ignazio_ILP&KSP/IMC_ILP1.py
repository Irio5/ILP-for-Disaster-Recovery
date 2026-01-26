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
    '~/Ignazio_ECOC_2025_0408/DCP1_Set_Creation/DCP1_Set1.csv',
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

    






def create_ilp_model(nodes_A, nodes_B, edges_A, edges_B, requests, capacities_A, 
                     capacities_B, prices_A, prices_B, normal_prices_A, normal_prices_B,
                     link_status_A, link_status_B, time_slots, carriers, 
                     weight1=1000000, weight2=0.01, weight3=100, weight4=1000, weight5=0.001):
    model = Model(name='ILP')

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

    
    # Binary variables: if a request is served
    #F = {σ: model.binary_var(name=f"F_{σ}") for σ in requests}
    # Fulfillment variables with carrier dimension
    F = {(σ): model.binary_var(name=f"F_{σ}") for σ in requests}


    # Bandwidth for request σ integer
    B = {σ: model.integer_var(lb=0, name=f"B_{σ}") for σ in requests}

    # Binary routing variables
    #x = {(e, σ): model.binary_var(name=f"x_{e[0]}_{e[1]}_{σ}") for e in bidirectional_edges for σ in requests}
    x = {(e, σ, a): model.binary_var(name=f"x_{e[0]}_{e[1]}_{σ}_{a}") for a in carriers for e in bidirectional_edges[a] for σ in requests}


    # Bandwidth on each link sum of x and B, integer
    #Y = {(e, σ): model.continuous_var(name=f"Y_{e[0]}_{e[1]}_{σ}") for e in bidirectional_edges for σ in requests}
    Y = {(e, σ, a): model.continuous_var(name=f"Y_{e[0]}_{e[1]}_{σ}_{a}") for a in carriers for e in bidirectional_edges[a] for σ in requests}
    

    # Binary variables for each DCP's topology
    G = {dcp: {g: model.binary_var(name=f"G_{dcp}_{g}") for g in dcp_groups[dcp]} for dcp in dcp_groups}

    # Define group fulfillment variables: G_fulfilled[dcp][g] represents the percentage of requests in group g of DCP dcp that are fulfilled
    G_fulfilled = {dcp: {g: model.continuous_var(lb=0, ub=1, name=f"G_fulfilled_{dcp}_{g}") for g in dcp_groups[dcp]} for dcp in dcp_groups}

    # Binary variables for link restoration
    T = model.integer_var(name=f"T")

    # Total number of links that need recovery to satisfy the request of the DCP
    Z = {n: model.integer_var(name=f"Z_{n}") for n in dcp_groups}

    # Binary recovery schedule for links
    Q = {(i, j, τ, a): model.binary_var(name=f"Q_{i}_{j}_{τ}_{a}") for a in carriers for (i, j) in bidirectional_edges[a] for τ in time_slots}


    # Flow constraints
    
    for σ in requests:
        source, destination, priority, bandwidth, tolerance, a = σ
        for i in nodes[a]:
            incoming_flow = model.sum(x[(j, i), σ, a] for j in nodes[a] if (j, i) in bidirectional_edges[a])
            outgoing_flow = model.sum(x[(i, j), σ, a] for j in nodes[a] if (j, i) in bidirectional_edges[a])
            if i == source:
                # Source node: flow originates from the source node
                model.add_constraint(outgoing_flow - incoming_flow == F[σ])
            elif i == destination:
                # Destination node: flow terminates at the destination node
                model.add_constraint(incoming_flow - outgoing_flow == F[σ])
            else:
                model.add_constraint(incoming_flow - outgoing_flow == 0)

    # Ensure that if F[σ, a] is 0, all corresponding x variables are also 0 NECESSARIO?
    for σ in requests:
        source, destination, priority, bandwidth, tolerance, carrier_id = σ
        for a in carriers:
            for e in bidirectional_edges[a]:
                model.add_constraint(x[e, σ, a] <= F[σ])


        
    # Only one topology per DCP is chosen
    for dcp in dcp_groups:
        model.add_constraint(model.sum(G[dcp][g] for g in dcp_groups[dcp]) == 1)
    
    
    # Constraints to determine group fulfillment percentage
    for dcp in dcp_groups:
        for g in dcp_groups[dcp]:
            for a in carriers:
                # G_fulfilled[dcp][g] represents the ratio of requests fulfilled to the total requests in the group
                model.add_constraint(G_fulfilled[dcp][g] == model.sum(F[σ] for σ in dcp_groups[dcp][g]) / len(dcp_groups[dcp][g]))
                model.add_constraint(G_fulfilled[dcp][g] <= G[dcp][g])
    
    
    
    # Only if the request belong to a positive group are positive
    for dcp in dcp_groups:
        for g in dcp_groups[dcp]:
            for σ in dcp_groups[dcp][g]:
                for a in carriers:
                    model.add_constraint(F[σ] <= G[dcp][g])
    
    
    
    # Bandwidth degradation tolerance constraints 
    for σ in requests:
        min_bw = σ[3] * (1 - σ[4])
        max_bw = σ[3]
        model.add_constraint(B[σ] >= min_bw * F[σ])
        model.add_constraint(B[σ] <= max_bw * F[σ])

    
    # Link Bandwidth assignment costraint
    M = 1e6  # A large number     
    for σ in requests:                   
        for a in carriers:
            for e in bidirectional_edges[a]:
                model.add_constraint(Y[e, σ, a] >= B[σ] - (1 - x[e, σ, a]) * M)
                model.add_constraint(Y[e, σ, a] <= B[σ]) 
                model.add_constraint(Y[e, σ, a] <= x[e, σ, a] * M) 
    
    
    # Capacity constraints 
    for a in carriers:
        for u, v in edges[a]: 
           model.add_constraint(
                model.sum(Y[(e, σ, a)] for e in [(u, v), (v, u)] for σ in requests) <= edge_capacities[a][(u, v)])
    
    
    # Constraint on the total number of damaged links which can be recovered per time slot
    for τ in time_slots:
        for a in carriers:
            model.add_constraint(model.sum(Q[i, j, τ, a] for (i, j) in edges[a]) <= 1)
    
    

    
    # Constraint that each damaged link can be recovered at most once per time slot
    for a in carriers:
        for (i, j) in bidirectional_edges[a]:
            model.add_constraint(model.sum(Q[i, j, τ, a] for τ in time_slots) <= 1)
            model.add_constraint(model.sum(Q[j, i, τ, a] for τ in time_slots) <= 1)


    
    
    
    # Constraint to limit total recovered links to 2 
    #model.add_constraint(model.sum(Q[i, j, τ] for (i, j) in bidirectional_edges[a] for τ in time_slots) <= 10)
    #model.add_constraint(model.sum(Q[i, j, τ] for (i, j) in bidirectional_edges[a] for τ in time_slots) >= 3)
    
    for a in carriers:
        for (i, j) in bidirectional_edges[a]:
            for τ in time_slots:
                # Ensure bidirectional synchronization in repairs
                model.add_constraint(Q[i, j, τ, a] == Q[j, i, τ, a])
                
    

    for a in carriers:
        for (i, j) in bidirectional_edges[a]:
            for σ in requests:           
                if link_status[a][(i, j)] > 0.5:
                    model.add_constraint(x[(i, j), σ, a]  <=  model.sum(Q[i, j, τ, a] for τ in time_slots))



    # Binary variables for first-time link usage
    first_use = {(i, j, a): model.binary_var(name=f"first_use_{i}_{j}_{a}") for a in carriers for (i, j) in bidirectional_edges[a]}

    # Add constraints to track first-time link usage
    for a in carriers:
        for (i, j) in bidirectional_edges[a]:
            # first_use is 1 if the link is used at least once
            model.add_constraint(first_use[i, j, a] <= model.sum(x[(i, j), σ, a] for σ in requests))
            # If first_use is 0, no request can use this link
            model.add_constraint(model.sum(x[(i, j), σ, a] for σ in requests) <= len(requests) * first_use[i, j, a])










    """
    # Combined Objective: maximize the number of requests served, total bandwidth, prioritize high-priority requests, and minimize total price, CORRECT WEIGHTS
    model.maximize(
        weight1 * model.sum(G_fulfilled[dcp][g] for dcp in dcp_groups for g in dcp_groups[dcp])
        + weight2 * model.sum(B[σ] for σ in requests)
        - weight3 * model.sum(Q[i, j, τ, a] for a in carriers for (i, j) in bidirectional_edges[a] for τ in time_slots)
        - weight4 * model.sum(σ[2] * F[σ] for σ in requests)  # σ[2] is the priority, occhio che favorisce i gruppi da 2 a quelli da 1
        #- weight5 * model.sum(x[e, σ, a] * edge_prices[a][e] for a in carriers for e in bidirectional_edges[a] for σ in requests)
        - weight5 * model.sum((first_use[i, j, a] * edge_prices[a][(i, j)] +
           (model.sum(x[(i, j), σ, a] for σ in requests) * edge_normal_prices[a][(i, j)] - first_use[i, j, a]) * edge_normal_prices[a][(i, j)]) 
           for a in carriers for (i, j)  in bidirectional_edges[a])- weight5 * model.sum((first_use[i, j, a] * edge_prices[a][(i, j)] +
           (model.sum(x[(i, j), σ, a] for σ in requests) * edge_normal_prices[a][(i, j)] - first_use[i, j, a]) * edge_normal_prices[a][(i, j)]) 
           for a in carriers for (i, j)  in bidirectional_edges[a]))









    
    """
    # Lexicographic Objective 1: Maximize group fulfillment (weight1)
    obj1 = model.sum(G_fulfilled[dcp][g] for dcp in dcp_groups for g in dcp_groups[dcp])
    model.maximize(obj1)

    # Solve first optimization
    solution1 = model.solve(log_output=True)
    if solution1:
        print("First Lexicographic Step: Group fulfillment maximized.")
        # Store the optimal objective value for group fulfillment
        optimal_obj1 = obj1.solution_value
        print(f"Optimal group fulfillment value: {optimal_obj1}")
    else:
        print("No solution found for the first lexicographic step.")
        return # Exit early if no solution is found

    # Lexicographic Objective 2: Maximize priority
    obj2 = model.sum(σ[2] * F[σ] for σ in requests)

    # Add a constraint to ensure that the first objective value is maintained
    model.add_constraint(obj1 == optimal_obj1)

    # Maximize the second objective
    model.minimize(obj2)

    # Solve second optimization
    solution2 = model.solve(log_output=True)
    if solution2:
        print("Second Lexicographic Step: Priority maximized.")
        # Store the optimal objective value for priorityyy
        optimal_obj2 = obj2.solution_value
        print(f"Optimal total priority value: {optimal_obj2}")
    else:
        print("No solution found for the second lexicographic step.")
        return  # Exit early if no solution is found

    # Lexicographic Objective 3: Minimize Restoration Time 
    obj3 = model.sum(Q[i, j, τ, a] for a in carriers for (i, j) in bidirectional_edges[a] for τ in time_slots)

    # Add a constraint to maintain the optimal value of the first two lexicographic objectives
    model.add_constraint(obj1 == optimal_obj1)
    model.add_constraint(obj2 == optimal_obj2)

    # Maximize the second objective
    model.minimize(obj3)

    # Solve second optimization
    solution3 = model.solve(log_output=True)
    if solution3:
        print("Third Lexicographic Step: Restoration time minimized.")
        # Store the optimal objective value for restoration time
        optimal_obj3 = obj3.solution_value
        print(f"Optimal total restoration time value: {optimal_obj3}")
    else:
        print("No solution found for the third lexicographic step.")
        return  # Exit early if no solution is found
    
    # Lexicographic Objective 4: Minimize Cost
    obj4 = model.sum((first_use[i, j, a] * edge_prices[a][(i, j)] +
           (model.sum(x[(i, j), σ, a] for σ in requests) * edge_normal_prices[a][(i, j)] - first_use[i, j, a]) * edge_normal_prices[a][(i, j)]) 
           for a in carriers for (i, j)  in bidirectional_edges[a])

    # Add a constraint to maintain the optimal value of the first two lexicographic objectives
    model.add_constraint(obj1 == optimal_obj1)
    model.add_constraint(obj2 == optimal_obj2)
    model.add_constraint(obj3 == optimal_obj3)

    # Maximize the second objective
    model.minimize(obj4)

    # Solve second optimization
    solution4 = model.solve(log_output=True)
    if solution4:
        print("Fourth Lexicographic Step: Cost minimized.")
        # Store the optimal objective value for restoration time
        optimal_obj4 = obj4.solution_value
        print(f"Optimal total restoration time value: {optimal_obj4}")
    else:
        print("No solution found for the fourth lexicographic step.")
        return  # Exit early if no solution is found

    # Final combined objective with the remaining weights (normal weighted sum for weight3, weight4, weight5)
    obj_remaining = (
        #- weight3 * model.sum(Q[i, j, τ, a] for a in carriers for (i, j) in bidirectional_edges[a] for τ in time_slots)
        + weight2 * model.sum((B[σ] for σ in requests))
        #- weight5 * model.sum(x[e, σ, a] * edge_prices[a][e] for a in carriers for e in bidirectional_edges[a] for σ in requests)
        #- weight5 * model.sum((first_use[i, j, a] * edge_prices[a][(i, j)] +
        #   (model.sum(x[(i, j), σ, a] for σ in requests) * edge_normal_prices[a][(i, j)] - first_use[i, j, a]) * edge_normal_prices[a][(i, j)]) 
        #   for a in carriers for (i, j)  in bidirectional_edges[a])
    )

    # Add a constraint to maintain the optimal value of the first two lexicographic objectives
    model.add_constraint(obj1 == optimal_obj1)
    model.add_constraint(obj2 == optimal_obj2)
    model.add_constraint(obj3 == optimal_obj3)
    model.add_constraint(obj4 == optimal_obj4)


    # Maximize the final weighted objective
    model.maximize(obj_remaining)



    

    # Solve the combined objective
    solution = model.solve(log_output=True)
    
    #print(solution)  
                                
    if solution:
        ilp_groups_valid = True
        #print("Solution status:", model.get_solve_status())
        #print("Objective value:", model.objective_value)
        for σ in requests:
            #print(f"Request {σ}: F = {F[σ].solution_value}, B = {B[σ].solution_value} required {σ[3]}")
            for a in carriers:
                for i,j in bidirectional_edges[a]:
                    if x[(i, j), σ, a].solution_value > 0.5:
                        #print(f"  Uses edge {i, j} of {a} with Y = {Y[(i, j), σ, a].solution_value} ")
                        if link_status[a][(i, j)] > 0.5:
                            #print(f"  Edge ({i}, {j}) requires recovery")
                            for τ in time_slots:
                                if Q[i, j, τ, a].solution_value > 0.5:
                                    #print(f"  Recovered ({i}, {j}) at time slot {τ}")
                                    continue



        for dcp in dcp_groups:
            for g in dcp_groups[dcp]:
                print(f"group_fulfillment_{G_fulfilled[dcp][g].solution_value} DCP {dcp} {g}")
                if sum(G_fulfilled[dcp][g].solution_value for g in dcp_groups[dcp]) <= 0.98: 
                    ilp_groups_valid = False
        

    else:
        print("No solution found for ILP")
    




    
# Prepare ILP results data similar to KSP format
    ilp_results_data_carrier1 = []
    ilp_results_data_carrier2 = []
    carrier1_id = 1
    carrier2_id = 1
    carrier1_lead_time = 0
    carrier2_lead_time = 0
    failed_links_carrier = {
        'carrier1': {},
        'carrier2': {}
    } 

    # Process all requests for ILP
    if solution:
        for σ in requests:
            # Check if request was accepted (F value > 0.5)
            if F[σ].solution_value > 0.5:
                source, destination, priority, bandwidth, degradation_tolerance, carrier_id = σ
                min_bw = int(bandwidth * (1 - degradation_tolerance))
                max_bw = bandwidth
                satisfied_bw = float(B[σ].solution_value)
                path_edges = []
                failed_links_list = []
                total_cost = 0
                total_normal_cost = 0
                current_lead_time = 0
                result = 1
                
                # Find all edges used for this request across both carriers
                for a in carriers:
                    for i, j in bidirectional_edges[a]:
                        if x[(i, j), σ, a].solution_value > 0.5:
                            path_edges.append((i, j, a))
                            total_cost += edge_prices[a][(i, j)]
                            total_normal_cost += edge_normal_prices[a][(i, j)]
                            
                            # Check if this is a failed link that was recovered
                            if link_status[a][(i, j)] > 0.5:
                                sorted_link = tuple(sorted([i, j]))
                                failed_links_list.append((i ,j))
                                result = 2

                                if sorted_link not in failed_links_carrier[a]:
                                    if carrier_id == 'carrier1':
                                        carrier1_lead_time += 1
                                        failed_links_carrier[a][sorted_link] = carrier1_lead_time
                                    else:
                                        carrier2_lead_time += 1
                                        failed_links_carrier[a][sorted_link] = carrier2_lead_time
                                link_lead_time = failed_links_carrier[a][sorted_link]
                                current_lead_time = max(current_lead_time, link_lead_time)

                # Build ordered path
                ordered_path = [int(source)]
                current_node = int(source)
                while current_node != int(destination):
                    next_found = False
                    for i, j, a in path_edges:
                        if int(i) == current_node:
                            ordered_path.append(int(j))
                            current_node = int(j)
                            next_found = True
                            break
                    if not next_found:
                        print(f"Warning: Could not complete path for request {source}->{destination}")
                        break

                # Format path string
                path_str = '->'.join(map(str, ordered_path))

                # Format failed links for CSV
                failed_links_str = ', '.join([f"{i}->{j}" for i, j in failed_links_list])
            

                # Get and increment the appropriate ID counter
                if carrier_id == 'carrier1':
                    current_id = carrier1_id
                    carrier1_id += 1
                else:
                    current_id = carrier2_id
                    carrier2_id += 1

                # Create row for CSV
                row = {
                    'ID': current_id,
                    'Src_node': source,
                    'Dst_node': destination,
                    'Min_Req_BW': min_bw,
                    'Max_Req_BW': max_bw,
                    'Satisfied_Bandwidth': int(satisfied_bw),
                    'Latency': 20,
                    'Result': result,
                    'Lead_Time': current_lead_time,
                    'Score': 100,
                    'Customer_ID': int('3') if carrier_id == 'carrier1' else '4',
                    'Priority': priority,
                    'Price': total_cost,
                    'Normal_Price': total_normal_cost,
                    'Used_F_Links': failed_links_str if failed_links_str else "None",
                    'Route': path_str
                }
                if carrier_id == 'carrier1' :
                    ilp_results_data_carrier1.append(row)
                else:
                    ilp_results_data_carrier2.append(row)


    # Write ILP results to CSV file
    csv_fields = ['ID', 'Src_node', 'Dst_node', 'Min_Req_BW', 'Max_Req_BW', 
                 'Satisfied_Bandwidth', 'Latency', 'Result', 'Lead_Time', 'Score',
                 'Customer_ID', 'Priority', 'Price', 'Normal_Price', 'Used_F_Links', 'Route']

    #Subhadeep CHECK output file folder
    ilp_csv_filename_carrier1 = "ilp1_carrier0_Final.csv"
    with open(ilp_csv_filename_carrier1, mode='w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=csv_fields)
        writer.writeheader()
        writer.writerows(ilp_results_data_carrier1)

    ilp_csv_filename_carrier2 = "ilp1_carrier1_Final.csv"
    with open(ilp_csv_filename_carrier2, mode='w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=csv_fields)
        writer.writeheader()
        writer.writerows(ilp_results_data_carrier2)
    




    



#create_ilp_model(nodes, edges, requests, capacities, prices, dcp_groups, time_slots, link_status)
create_ilp_model(nodes_A, nodes_B, edges_A, edges_B, requests, capacities_A, capacities_B, prices_A, prices_B, 
                        normal_prices_A, normal_prices_B, link_status_A, link_status_B, time_slots, carriers)



