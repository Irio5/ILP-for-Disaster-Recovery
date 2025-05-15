import networkx as nx
import pandas as pd
import matplotlib.pyplot as plt

"""
Carrier_0 Network
"""

carrier_0_links = pd.read_csv("Carrier_0_links.csv")   #read links file
carrier_0_nodes = pd.read_csv("Carrier_0_nodes.csv")   #read nodes file


list_carrier_0_nodes = carrier_0_nodes['ID'].tolist()
list_carrier_0_links_i = carrier_0_links['Src_node'].tolist()
list_carrier_0_links_j = carrier_0_links['Dst_node'].tolist()
list_carrier_0_links_ID = carrier_0_links['ID'].tolist()
list_carrier_0_links_Flag = carrier_0_links['Flag'].tolist()


carrier_0_network = nx.Graph()   #Build the DCI network
carrier_0_network.add_nodes_from(list_carrier_0_nodes)
for idx, i in enumerate(carrier_0_links['ID']):
    carrier_0_network.add_edge(list_carrier_0_links_i[idx], list_carrier_0_links_j[idx], ID=list_carrier_0_links_ID[idx], Flag=list_carrier_0_links_Flag[idx])

#print(carrier_0_network)
plt.clf()
nx.draw_spring(carrier_0_network, with_labels=True, node_color='red')
plt.savefig("carrier_0_network.png")


"""
Carrier_1 Network
"""

carrier_1_links = pd.read_csv("Carrier_1_links.csv")   #read links file
carrier_1_nodes = pd.read_csv("Carrier_1_nodes.csv")   #read nodes file


list_carrier_1_nodes = carrier_1_nodes['ID'].tolist()
list_carrier_1_links_i = carrier_1_links['Src_node'].tolist()
list_carrier_1_links_j = carrier_1_links['Dst_node'].tolist()
list_carrier_1_links_ID = carrier_1_links['ID'].tolist()
list_carrier_1_links_Flag = carrier_1_links['Flag'].tolist()


carrier_1_network = nx.Graph()   #Build the DCI network
carrier_1_network.add_nodes_from(list_carrier_1_nodes)
for idx, i in enumerate(carrier_1_links['ID']):
    carrier_1_network.add_edge(list_carrier_1_links_i[idx], list_carrier_1_links_j[idx], ID=list_carrier_1_links_ID[idx], Flag=list_carrier_1_links_Flag[idx])

#print(carrier_1_network)
plt.clf()
nx.draw_spring(carrier_1_network, with_labels=True, node_color='blue')
plt.savefig("carrier_1_network.png")


"""
create price Dataframe  Path-ID Source Dest Carrier-ID Price
"""""
price_data = {'Path-ID': [9999], 'Source': [9999], 'Dest': [9999], 'Carrier-ID': [9999], 'Price': [9999], 'Normal_Price': [9999]}
price_df = pd.DataFrame(price_data)
rows_count = price_df.shape[0]
price_df = price_df.drop(price_df.index[range(rows_count)])


"""
create price Dataframe  Path-ID Source Dest Carrier-ID Price
"""""
price_data1 = {'Path-ID': [9999], 'Source': [9999], 'Dest': [9999], 'Carrier-ID': [9999], 'Price': [9999]}
price_df1 = pd.DataFrame(price_data1)
rows_count1 = price_df1.shape[0]
price_df1 = price_df1.drop(price_df1.index[range(rows_count1)])

"""
create path ID Dataframe
"""""
path_data = {'Path-ID': [9999], 'Source': [9999], 'Dest': [9999], 'Carrier-ID': [9999], 'Price': [9999], 'Normal_Price': [9999]}
path_df = pd.DataFrame(path_data)
rows_count2 = path_df.shape[0]
path_df = path_df.drop(path_df.index[range(rows_count2)])


"""
create path ID Dataframe for Text
"""""
path_data1 = {'Path-ID': [9999], 'Source': [9999], 'Dest': [9999], 'Carrier-ID': [9999], 'Price': [9999]}
path_df1 = pd.DataFrame(path_data1)
rows_count3 = path_df1.shape[0]
path_df1 = path_df1.drop(path_df1.index[range(rows_count3)])


"""
Price generation with 3 paths
"""
for i in list_carrier_0_nodes:
    for j in list_carrier_0_nodes:
        if i != j:
            path_price = 0
            min_price = 0
            norm_price = 0
            #path = nx.dijkstra_path(carrier_0_network, i, j)
            path_set = nx.shortest_simple_paths(carrier_0_network, i, j)
            k = 3
            list_price = []
            for counter, path_i in enumerate(path_set):
                path_price = 0
                norm_price = 0
                path_ID = 0
                for index_j in range(len(path_i)):

                    if index_j + 1 == len(path_i):
                        break
                    else:
                        flag = carrier_0_network.get_edge_data(path_i[index_j], path_i[index_j + 1]).get('Flag')
                        if flag == 1:
                            path_price = path_price + 2
                            norm_price = norm_price + 2
                        else:
                            path_price = path_price + 52
                            norm_price = norm_price + 2

                path_ID = counter + 1
                path_row = {'Path-ID': path_ID, 'Source': i, 'Dest': j, 'Carrier-ID': 0, 'Price': path_price, 'Normal_Price': norm_price}
                path_row_df = pd.DataFrame([path_row])
                path_df = pd.concat([path_df, path_row_df], ignore_index=True)
                #path_df = path_df.append(path_row, ignore_index=True) #old

                list_price.append(path_price)
                if counter == k - 1:
                    break

            min_price = min(list_price)
            path_number = path_df.loc[
                (path_df['Source'] == i) & (path_df['Dest'] == j) & (path_df['Carrier-ID'] == 0) & (
                            path_df['Price'] == min_price), 'Path-ID'].iloc[0]
            norm_price1 = path_df.loc[
                (path_df['Source'] == i) & (path_df['Dest'] == j) & (path_df['Carrier-ID'] == 0) & (
                        path_df['Price'] == min_price), 'Normal_Price'].iloc[0]
            add_row = {'Path-ID': path_number, 'Source': i, 'Dest': j, 'Carrier-ID': 0, 'Price': min_price, 'Normal_Price': norm_price1}
            #add_row = {'Name': 0, 'Src_node': i, 'Dst_node': j, 'Price': min_price}
            rows_count1 = path_df.shape[0]
            path_df = path_df.drop(path_df.index[range(rows_count1)])
            add_row_df = pd.DataFrame([add_row])
            price_df = pd.concat([price_df, add_row_df], ignore_index=True)
            #price_df = price_df.append(add_row, ignore_index=True) #old
        else:
            dummy_row = {'Path-ID': 999, 'Source': i, 'Dest': j, 'Carrier-ID': 0, 'Price': 999, 'Normal_Price': 999}
            dummy_row_df = pd.DataFrame([dummy_row])
            price_df = pd.concat([price_df, dummy_row_df], ignore_index=True)
            #price_df = price_df.append(dummy_row, ignore_index=True) #old

for i in list_carrier_1_nodes:
    for j in list_carrier_1_nodes:
        if i != j:
            path_price = 0
            min_price = 0
            norm_price = 0
            #path = nx.dijkstra_path(carrier_0_network, i, j)
            path_set = nx.shortest_simple_paths(carrier_1_network, i, j)
            k = 3
            list_price = []
            for counter, path_i in enumerate(path_set):
                path_price = 0
                norm_price = 0
                path_ID = 0
                for index_j in range(len(path_i)):

                    if index_j + 1 == len(path_i):
                        break
                    else:
                        flag = carrier_1_network.get_edge_data(path_i[index_j], path_i[index_j + 1]).get('Flag')
                        if flag == 1:
                            path_price = path_price + 2
                            norm_price = norm_price + 2
                        else:
                            path_price = path_price + 52
                            norm_price = norm_price + 2

                path_ID = counter + 1
                path_row = {'Path-ID': path_ID, 'Source': i, 'Dest': j, 'Carrier-ID': 1, 'Price': path_price, 'Normal_Price': norm_price}
                path_row_df = pd.DataFrame([path_row])
                path_df = pd.concat([path_df, path_row_df], ignore_index=True)
                # path_df = path_df.append(path_row, ignore_index=True) #old

                list_price.append(path_price)
                if counter == k - 1:
                    break

            min_price = min(list_price)
            path_number = path_df.loc[
                (path_df['Source'] == i) & (path_df['Dest'] == j) & (path_df['Carrier-ID'] == 1) & (
                            path_df['Price'] == min_price), 'Path-ID'].iloc[0]
            norm_price1 = path_df.loc[
                (path_df['Source'] == i) & (path_df['Dest'] == j) & (path_df['Carrier-ID'] == 1) & (
                        path_df['Price'] == min_price), 'Normal_Price'].iloc[0]
            add_row = {'Path-ID': path_number, 'Source': i, 'Dest': j, 'Carrier-ID': 1, 'Price': min_price, 'Normal_Price': norm_price1}
            #add_row = {'Name': 0, 'Src_node': i, 'Dst_node': j, 'Price': min_price}
            rows_count1 = path_df.shape[0]
            path_df = path_df.drop(path_df.index[range(rows_count1)])
            add_row_df = pd.DataFrame([add_row])
            price_df = pd.concat([price_df, add_row_df], ignore_index=True)
            # price_df = price_df.append(add_row, ignore_index=True) #old
        else:
            dummy_row = {'Path-ID': 999, 'Source': i, 'Dest': j, 'Carrier-ID': 1, 'Price': 999, 'Normal_Price': 999}
            dummy_row_df = pd.DataFrame([dummy_row])
            price_df = pd.concat([price_df, dummy_row_df], ignore_index=True)
            # price_df = price_df.append(dummy_row, ignore_index=True) #old






price_df.to_csv("Price_lists.csv", index=False)

rows_count = price_df.shape[0]
price_df = price_df.drop(price_df.index[range(rows_count)])

"""
Price generation with 3 paths for .text format
"""

total_path = 0
path_number = 0
for i in list_carrier_0_nodes:
    for j in list_carrier_0_nodes:
        if (j > i) & (j != i):
            path_price = 0
            min_price = 0
            norm_price = 0
            # path = nx.dijkstra_path(carrier_0_network, i, j)
            path_set = nx.shortest_simple_paths(carrier_0_network, i, j)
            k = 3
            list_price = []
            for counter, path_i in enumerate(path_set):
                path_price = 0
                norm_price = 0
                path_ID = 0
                for index_j in range(len(path_i)):

                    if index_j + 1 == len(path_i):
                        break
                    else:
                        flag = carrier_0_network.get_edge_data(path_i[index_j], path_i[index_j + 1]).get('Flag')
                        if flag == 1:
                            path_price = path_price + 2
                            norm_price = norm_price + 2
                        else:
                            path_price = path_price + 52
                            norm_price = norm_price + 2

                path_ID = counter + 1
                path_row = {'Path-ID': path_number, 'Source': i, 'Dest': j, 'Carrier-ID': 0, 'Price': path_price,
                            'Normal_Price': norm_price}
                path_row_df = pd.DataFrame([path_row])
                path_df = pd.concat([path_df, path_row_df], ignore_index=True)
                # path_df = path_df.append(path_row, ignore_index=True) #old

                list_price.append(path_price)
                if counter == k - 1:
                    break

            min_price = min(list_price)

            norm_price1 = path_df.loc[
                (path_df['Source'] == i) & (path_df['Dest'] == j) & (path_df['Carrier-ID'] == 0) & (
                        path_df['Price'] == min_price), 'Normal_Price'].iloc[0]
            add_row = {'Path-ID': path_number, 'Source': i, 'Dest': j, 'Carrier-ID': 0, 'Price': min_price,
                       'Normal_Price': norm_price1}
            # add_row = {'Name': 0, 'Src_node': i, 'Dst_node': j, 'Price': min_price}
            rows_count1 = path_df.shape[0]
            path_df = path_df.drop(path_df.index[range(rows_count1)])
            add_row_df = pd.DataFrame([add_row])
            price_df = pd.concat([price_df, add_row_df], ignore_index=True)
            #price_df = price_df.append(add_row, ignore_index=True) #old
            total_path = total_path + 1
            path_number = path_number + 1


for i in list_carrier_1_nodes:
    for j in list_carrier_1_nodes:
        if (j > i) & (j != i):
            path_price = 0
            min_price = 0
            norm_price = 0
            # path = nx.dijkstra_path(carrier_0_network, i, j)
            path_set = nx.shortest_simple_paths(carrier_1_network, i, j)
            k = 3
            list_price = []
            for counter, path_i in enumerate(path_set):
                path_price = 0
                norm_price = 0
                path_ID = 0
                for index_j in range(len(path_i)):

                    if index_j + 1 == len(path_i):
                        break
                    else:
                        flag = carrier_1_network.get_edge_data(path_i[index_j], path_i[index_j + 1]).get('Flag')
                        if flag == 1:
                            path_price = path_price + 2
                            norm_price = norm_price + 2
                        else:
                            path_price = path_price + 52
                            norm_price = norm_price + 2

                path_ID = counter + 1
                path_row = {'Path-ID': path_number, 'Source': i, 'Dest': j, 'Carrier-ID': 1, 'Price': path_price,
                            'Normal_Price': norm_price}
                path_row_df = pd.DataFrame([path_row])
                path_df = pd.concat([path_df, path_row_df], ignore_index=True)
                # path_df = path_df.append(path_row, ignore_index=True) #old

                list_price.append(path_price)
                if counter == k - 1:
                    break

            min_price = min(list_price)

            norm_price1 = path_df.loc[
                (path_df['Source'] == i) & (path_df['Dest'] == j) & (path_df['Carrier-ID'] == 1) & (
                        path_df['Price'] == min_price), 'Normal_Price'].iloc[0]
            add_row = {'Path-ID': path_number, 'Source': i, 'Dest': j, 'Carrier-ID': 1, 'Price': min_price,
                       'Normal_Price': norm_price1}
            # add_row = {'Name': 0, 'Src_node': i, 'Dst_node': j, 'Price': min_price}
            rows_count1 = path_df.shape[0]
            path_df = path_df.drop(path_df.index[range(rows_count1)])
            add_row_df = pd.DataFrame([add_row])
            price_df = pd.concat([price_df, add_row_df], ignore_index=True)
            # price_df = price_df.append(add_row, ignore_index=True) #old
            total_path = total_path + 1
            path_number = path_number + 1






#price_df.to_csv("111Price_lists.txt", index=False)


file = open("Price_lists.txt","r+")
file.truncate(0)
file.close()
price_df.to_csv('Price_lists.txt', header=True, index=False, sep='\t', mode='a')


src = open("Price_lists.txt", "r")
#fline = "Np#=11 Path#=3\n"  # Prepending string
oline = src.readlines()
#oline.insert(0, fline)
oline.insert(0, "Np#=11 Path#=%d\n" % total_path)
src.close()
src = open("Price_lists.txt", "w")
src.writelines(oline)
src.close()

