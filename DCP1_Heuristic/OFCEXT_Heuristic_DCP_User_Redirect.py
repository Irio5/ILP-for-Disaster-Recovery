

#Help here- https://medium.com/opex-analytics/optimization-modeling-in-python-pulp-gurobi-and-cplex-83a62129807a


#from docplex.mp.model import Model
import math
import networkx as nx
import pandas as pd
#import numpy as np
import matplotlib.pyplot as plt
import random
import csv
pd.options.mode.chained_assignment = None

"""
Designing the Network
"""

links = pd.read_csv("links_DCP1_AD.csv")   #read links file
print(links.loc[(links['Src_node'] == 1) & (links['Dst_node'] == 2), 'ID'])

nodes = pd.read_csv("nodes_DCP1_AD.csv")   #read nodes file
#list = nodes.loc[nodes['ID'] == 2, ['P_contents', 'B_contents']]

list_nodes = nodes['ID'].tolist()
list_links_i = links['Src_node'].tolist()
list_links_j = links['Dst_node'].tolist()
list_links_ID = links['ID'].tolist()
list_links_BW = links['Satisfied_Bandwidth'].tolist()


network = nx.Graph()   #Build the DCI network
network.add_nodes_from(list_nodes)
for idx, i in enumerate(links['ID']):
    network.add_edge(list_links_i[idx], list_links_j[idx], ID=list_links_ID[idx], BW=list_links_BW[idx])

print(network)
plt.clf()
nx.draw_spring(network, with_labels=True, node_color='red')
plt.savefig("DCP1_AD.png")

print("Edges in DCI: ", network.edges(data=True))
print(network.edges(1, 'BW'))
print(network.edges)
#print(network.get_edge_data(1, 2).values())
#print(network.get_edge_data(2, 5).get('BW'))
#network[2][5]['BW'] = 1110101

#print('Updated BW:', network.get_edge_data(2, 5).get('BW'))
print(network.nodes())
network_nodes_arr = [int(item) for item in network.nodes()]

"""
Designing the Network Before Disaster
"""

links_BD = pd.read_csv("links_DCP1_BD.csv")   #read links file
#print(links.loc[(links['Src_node'] == 1) & (links['Dst_node'] == 2), 'ID'])

nodes_BD = pd.read_csv("nodes_DCP1_BD.csv")   #read nodes file
#list = nodes.loc[nodes['ID'] == 2, ['P_contents', 'B_contents']]

list_nodes_BD = nodes_BD['ID'].tolist()
list_links_i_BD = links_BD['Src_node'].tolist()
list_links_j_BD = links_BD['Dst_node'].tolist()
list_links_ID_BD = links_BD['ID'].tolist()
list_links_BW_BD = links_BD['Bandwidth'].tolist()


network_BD = nx.Graph()   #Build the DCI network
network_BD.add_nodes_from(list_nodes_BD)
for idx, i in enumerate(links_BD['ID']):
    network_BD.add_edge(list_links_i_BD[idx], list_links_j_BD[idx], ID=list_links_ID_BD[idx], BW=list_links_BW_BD[idx])

print(network_BD)
plt.clf()
nx.draw_spring(network_BD, with_labels=True, node_color='lightgreen')
plt.savefig("DCP1_BD.png")


"""
Reading user requests
"""
requests = []
with open('Final_content_requests.csv', mode='r') as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        source = row['Source']
        destinations = row['Destinations'].split(';')
        data = int(row['Required_BW'])

        requests.append({
            'source': source,
            'destinations': destinations,
            'data': data
        })



"""
Designing carriers
"""
carriers_list = [0, 1]
carriers_bible = pd.read_csv("Price_lists.csv")


"""
Designing contents
"""
content_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
contents = pd.read_csv("contents_primary_DCP1.csv")
print(contents.head())

"""
Loading Lightpath Request File
"""
lightpath_req = pd.read_csv("LightpathReq.csv")
lightpath_req['Min_Required_Lightpath_Bandwidth'] = 0
lightpath_req['Max_Required_Lightpath_Bandwidth'] = 0
lightpath_req['Carrier_ID'] = 0
lightpath_req['Users_count'] = 0
lightpath_req['Priority_1'] = 0
lightpath_req['Priority_2'] = 0
lightpath_req['Priority_3'] = 0

lightpath_req.to_csv("LightpathReq.csv", index=False)

"""
create price Dataframe
"""""
price_data = {'for_user_ID': [99999], 'carrier_ID': [9999], 'src_ID': [9999], 'inter_node_ID': [9999], 'dst_ID': [9999], 'price': [9999], 'Content_redis': [9999]}
price_df = pd.DataFrame(price_data)

#rows_count_price = price_df.shape[0]
#price_df = price_df.drop(price_df.index[range(rows_count_price)])

"""
create path Dataframe
"""""
#path_data = {'path_ID': [9999], 'carrier_ID': [9999], 'src_ID': [9999], 'path': [9999, 999, 999]}
#path_df = pd.DataFrame(path_data)
#path_df.to_csv("paths.csv", index=False)
"""
Designning the Heuristic
"""""
print(network.edges(data=True))
#lightpath_req_Dict = {}
ip_restored = 0
lightpath_restored = 0

for r in range(len(requests)):
    print('\n')
    print('Working with content request:', r)
    break_out_flag = False
    src = requests[r]['source']
    req_data = requests[r]['data']
    src_node = int(src)
    #final_dest = 0

    for d in requests[r]['destinations']:

        d_node = int(d)
        print('Looking for path in survived DCI topology')

        if nx.has_path(network, src_node, d_node):
            print("Path Found")
            first_path = nx.dijkstra_path(network, src_node, d_node)
            # print('ID: ', n, required_content, required_BW, host_dc, backup_dc)

            # Adding more paths scenario
            path_set = nx.shortest_simple_paths(network, src_node, d_node)
            k = 3
            for counter, path_i in enumerate(path_set):
                print('The path is:', path_i, 'The path number is: ', counter + 1)
                available_BW = int
                list_BW = []
                for index_i in range(len(path_i)):
                    # print(F[index_i])
                    if index_i + 1 == len(path_i):
                        break
                    else:
                        available_BW = network.get_edge_data(path_i[index_i], path_i[index_i + 1]).get('BW')
                        # print('available_BW', path[index_i], 'between', path[index_i + 1], '=', available_BW)
                        list_BW.append(available_BW)

                print(list_BW, 'Mbps')
                print('Min BW on this path is:', min(list_BW), 'Mbps')
                """
                if (counter == k - 1) & (min(list_BW) <= required_BW):
                    break
                else:
                    print("do")
                """
                if min(list_BW) >= req_data:
                    ip_restored = ip_restored + 1
                    print('content request', r, 'is restored using Path:', path_i)
                    for index_i in range(len(path_i)):
                        # print(F[index_i])
                        if index_i + 1 == len(path_i):
                            break
                        else:
                            network[path_i[index_i]][path_i[index_i + 1]]['BW'] = available_BW - req_data

                            break_out_flag = True
                else:
                    print('content request', r, 'cannot be restored using Path: ', counter + 1, 'due to less bandwidth')

                if break_out_flag == True:
                    break

                if counter == k - 1:
                    break

            if break_out_flag == True:
                break

        if break_out_flag == True:
            break

        """
        else:
            print("Path not Found, network disconnected")
            lightpath_restored = lightpath_restored + 1
            for a in carriers_list:
                # src_node, d_node
                direct_price = carriers_bible.loc[
                    (carriers_bible['Carrier-ID'] == a) & (carriers_bible['Source'] == src_node) & (
                                carriers_bible['Dest'] == d_node), 'Price'].iloc[0]
                direct_row = {'for_user_ID': r, 'carrier_ID': a, 'src_ID': src_node, 'inter_node_ID': 999,
                              'dst_ID': d_node, 'price': direct_price, 'Content_redis': 0}
                direct_row_todf = pd.DataFrame([direct_row])
                price_df = pd.concat([price_df, direct_row_todf], ignore_index=True)
                
        """

    if break_out_flag == False:
        lightpath_restored = lightpath_restored + 1
        for d in requests[r]['destinations']:
            des_node = int(d)
            print("Path not Found, network disconnected")
            #lightpath_restored = lightpath_restored + 1
            for a in carriers_list:
                # src_node, d_node
                direct_price = carriers_bible.loc[
                    (carriers_bible['Carrier-ID'] == a) & (carriers_bible['Source'] == src_node) & (
                            carriers_bible['Dest'] == des_node), 'Price'].iloc[0]
                direct_row = {'for_user_ID': r, 'carrier_ID': a, 'src_ID': src_node, 'inter_node_ID': 999,
                              'dst_ID': des_node, 'price': direct_price, 'Content_redis': 0}
                direct_row_todf = pd.DataFrame([direct_row])
                price_df = pd.concat([price_df, direct_row_todf], ignore_index=True)


        min_price = price_df.loc[price_df['for_user_ID'] == r, 'price'].min()
        print('Min price for content request ', r, 'is: ', min_price)
        #min_price = price_df['price'].min()
        filtered_dest_df = price_df.loc[(price_df['price'] == min_price) & (price_df['for_user_ID'] == r), 'dst_ID']
        #final_dest = price_df.loc[(price_df['price'] == min_price) & (price_df['for_user_ID'] == r) & (price_df['carrier_ID'] == 0), 'dst_ID'].iloc[0]
        #print('filtered_dest_df', filtered_dest_df)
        if not filtered_dest_df.empty:
            final_dest = filtered_dest_df.iloc[0]
            #print('final dest', final_dest)
        else:
            final_dest = None

        print('Min price for content request ', r, 'is: ', min_price, 'between', src_node, ' and ', final_dest)
        # print(price_df.index)
        #print(price_df)
        #print('Number of rows now: ', price_df.shape[0])
        # result_df = price_df.loc[price_df['price'] == min_price]
        # print(result_df)
        #inter_node = price_df.loc[price_df['price'] == min_price, 'inter_node_ID'].iloc[0]
        #content_flag = price_df.loc[price_df['price'] == min_price, 'Content_redis'].iloc[0]
        #print(inter_node, content_flag)
        temp_1 = lightpath_req.loc[
            ((lightpath_req['Src_node'] == src_node) & (lightpath_req['Dst_node'] == final_dest)) | (
                    (lightpath_req['Src_node'] == final_dest) & (
                    lightpath_req['Dst_node'] == src_node)), 'Min_Required_Lightpath_Bandwidth'].iloc[0]
        # print('Test Temp before:', temp)
        temp_1 = temp_1 + req_data
        # print('Test Temp after:', temp)
        lightpath_req.loc[((lightpath_req['Src_node'] == src_node) & (
                lightpath_req['Dst_node'] == final_dest)) | (
                                  (lightpath_req['Src_node'] == final_dest) & (
                                  lightpath_req['Dst_node'] == src_node)), 'Min_Required_Lightpath_Bandwidth'] = temp_1

        lightpath_req.loc[((lightpath_req['Src_node'] == src_node) & (
                lightpath_req['Dst_node'] == final_dest)) | (
                                  (lightpath_req['Src_node'] == final_dest) & (
                                  lightpath_req['Dst_node'] == src_node)), 'Max_Required_Lightpath_Bandwidth'] = temp_1

        temp_users = lightpath_req.loc[((lightpath_req['Src_node'] == src_node) & (
                lightpath_req['Dst_node'] == final_dest)) | (
                                               (lightpath_req['Src_node'] == final_dest) & (
                                               lightpath_req[
                                                   'Dst_node'] == src_node)), 'Users_count'].iloc[0]
        temp_users = temp_users + 1
        # print('Test Temp after:', temp)
        lightpath_req.loc[((lightpath_req['Src_node'] == src_node) & (
                lightpath_req['Dst_node'] == final_dest)) | (
                                  (lightpath_req['Src_node'] == final_dest) & (
                                  lightpath_req['Dst_node'] == src_node)), 'Users_count'] = temp_users



lightpath_req.to_csv("LightpathReq.csv", index=False)
column_name = 'for_user_ID'
price_df = price_df[price_df[column_name] != 99999]

price_df.to_csv("price_df_Checking.csv", index=False)


result_df = lightpath_req.loc[(lightpath_req['Max_Required_Lightpath_Bandwidth'] > 10) & (lightpath_req['Max_Required_Lightpath_Bandwidth'] < 500000)]
result_df['Number of lightpaths'] = 0
result_df['Payment'] = 0
result_df['Flat_Payment'] = 0
result_df['Request_Priority'] = 0

print(result_df)

for i in range(len(result_df)):
    #print(i, result_df.iloc[i, 1])
    s_node = result_df.iloc[i, 0]
    d_node = result_df.iloc[i, 1]
    Min_req_bw = math.ceil(result_df.iloc[i, 4] / 1)
    Max_req_bw = math.ceil(result_df.iloc[i, 5] / 1)
    print(s_node, "   ", d_node)
    #adding Priority
    u_count = result_df.iloc[i, 3]
    pr_1 = result_df.iloc[i, 6]
    pr_2 = result_df.iloc[i, 7]
    pr_3 = result_df.iloc[i, 8]

    #combined_priority = (0.5*u_count) + (0.3*pr_1) + (0.15*pr_2) + (0.5*pr_3)
    combined_priority = 1

    result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Request_Priority'] = combined_priority
    result_df.loc[(result_df['Src_node'] == s_node) & (
                result_df['Dst_node'] == d_node), 'Min_Required_Lightpath_Bandwidth'] = Min_req_bw
    result_df.loc[(result_df['Src_node'] == s_node) & (
                result_df['Dst_node'] == d_node), 'Max_Required_Lightpath_Bandwidth'] = Max_req_bw
    num_lightpaths = math.ceil(Max_req_bw/100000)

    temp_price_1 = 0
    temp_price_2 = 0
    temp_flat_1 = 0
    temp_flat_2 = 0
    #for a in carriers_list:
    for j in range(len(carriers_bible)):
        if (carriers_bible.iloc[j, 3] == 0) & (carriers_bible.iloc[j, 1] == s_node) & (carriers_bible.iloc[j, 2] == d_node):
            print("success 1")
            temp_price_1 = carriers_bible.iloc[j, 4]
            temp_flat_1 = carriers_bible.iloc[j, 5]
        elif (carriers_bible.iloc[j, 3] == 1) & (carriers_bible.iloc[j, 1] == s_node) & (carriers_bible.iloc[j, 2] == d_node):
            print("success  2")
            temp_price_2 = carriers_bible.iloc[j, 4]
            temp_flat_2 = carriers_bible.iloc[j, 5]


    if temp_price_1 > temp_price_2:
        print('lowest price from carrier 1 = ', temp_price_2)
        #result_df.iloc[i, 3] = 2
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = 1
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_2
        #result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Flat_Payment'] = temp_flat_2
    elif temp_price_1 == temp_price_2:
        print('lowest price from carrier 1 = ', temp_price_2)
        list_car = [1, 0]
        c_id = random.choice(list_car)
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = c_id
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_2
        #result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Flat_Payment'] = temp_flat_2
    else:
        print('lowest price from carrier 0 = ', temp_price_1)
        #result_df.iloc[i, 3] = 1
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = 0
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_1
        #result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Flat_Payment'] = temp_flat_1

    result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Number of lightpaths'] = num_lightpaths
    result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node) & (result_df['Carrier_ID'] == 0), 'Flat_Payment'] = temp_flat_1 * num_lightpaths
    result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node) & (result_df['Carrier_ID'] == 1), 'Flat_Payment'] = temp_flat_2 * num_lightpaths



print(result_df)
final_result_df = result_df.sort_values(by=['Request_Priority'], ascending=False)
final_result_df.reset_index(inplace=True)
final_result_df['Final_Priority'] = 1
final_result_df = final_result_df.drop(columns='index')
print(final_result_df)

final_result_df.to_csv("Pre_Final_LightpathReq_DCP1.csv", index=False)

"""
Reseting Priorities for both the Carriers
"""
reset_prior_df = pd.read_csv("Pre_Final_LightpathReq_DCP1.csv")
df_carrier_0 = reset_prior_df
df_carrier_1 = reset_prior_df
for i in range(len(reset_prior_df)):

    #carrier_id = reset_prior_df.iloc[i, 2]
    #calculated_priority = reset_prior_df.iloc[i, 13]
    #result_df = lightpath_req.loc[(lightpath_req['Max_Required_Lightpath_Bandwidth'] > 10) & (
    #           lightpath_req['Max_Required_Lightpath_Bandwidth'] < 500000)]
    df_carrier_0 = reset_prior_df.loc[reset_prior_df['Carrier_ID'] == 0]
    df_carrier_1 = reset_prior_df.loc[reset_prior_df['Carrier_ID'] == 1]

df_carrier_0.reset_index(inplace=True)
df_carrier_0['Final_Priority'] = 1
#df_carrier_0['Path_ID'] = df_carrier_0.index
df_carrier_0 = df_carrier_0.drop(columns='index')


df_carrier_1.reset_index(inplace=True)
df_carrier_1['Final_Priority'] = 1
#df_carrier_1['Path_ID'] = df_carrier_1.index
df_carrier_1 = df_carrier_1.drop(columns='index')

combine_final_req = pd.concat([df_carrier_0, df_carrier_1], ignore_index=True)
combine_final_req.to_csv("DCP1_Set1.csv", index=False)

print("this is carrier 0")
print(df_carrier_0)
print("this is carrier 1")
print(df_carrier_1)
"""
End of Reseting Priorities for both the Carriers
"""


print(network.edges(data=True))
print('IP layer restored requests:', ip_restored)
print("New connection required to restore connections: ", lightpath_restored)

IPlight_data = {'DCI_Layer': ip_restored, 'New_connection_req': lightpath_restored}
IPlight_data_df = pd.DataFrame([IPlight_data])
IPlight_data_df.to_csv("Service_Restoration_Content_User_requests.csv", index=False)



"""
Requests to carriers using the format 
#Now will be moved to Combining python file
"""
dcp_1_set_1 = pd.read_csv("DCP1_Set1.csv")

for i in range(len(links)):
    s_node_org = links.iloc[i, 1]
    d_node_org = links.iloc[i, 2]
    Min_req_bw_org = 100000
    Max_req_bw_org = 100000
    u_count_org = 0
    Carr_ID = links.iloc[i, 11]

    direct_row = {'Src_node': s_node_org, 'Dst_node': d_node_org, 'Carrier_ID': Carr_ID, 'Users_count': 0,
                  'Min_Required_Lightpath_Bandwidth': Min_req_bw_org, 'Max_Required_Lightpath_Bandwidth': Max_req_bw_org,
                  'Priority_1': 0, 'Priority_2': 0, 'Priority_3': 0, 'Number of lightpaths': 1, 'Payment': 2,
                  'Flat_Payment': 2, 'Request_Priority': 1, 'Final_Priority': 1}
    direct_row_todf = pd.DataFrame([direct_row])
    dcp_1_set_1 = pd.concat([dcp_1_set_1, direct_row_todf], ignore_index=True)



dcp_1_set_1['From_DCP'] = 2
reset_path_ID_df = dcp_1_set_1
for i in range(len(reset_path_ID_df)):

    #carrier_id = reset_prior_df.iloc[i, 2]
    #calculated_priority = reset_prior_df.iloc[i, 13]
    #result_df = lightpath_req.loc[(lightpath_req['Max_Required_Lightpath_Bandwidth'] > 10) & (
    #           lightpath_req['Max_Required_Lightpath_Bandwidth'] < 500000)]
    df_carrier_0_pathID = reset_path_ID_df.loc[reset_path_ID_df['Carrier_ID'] == 0]
    df_carrier_1_pathID = reset_path_ID_df.loc[reset_path_ID_df['Carrier_ID'] == 1]

df_carrier_0_pathID.reset_index(inplace=True)
df_carrier_0_pathID['Path_ID'] = df_carrier_0_pathID.index + 0
df_carrier_0_pathID = df_carrier_0_pathID.drop(columns='index')


df_carrier_1_pathID.reset_index(inplace=True)
df_carrier_1_pathID['Path_ID'] = df_carrier_1_pathID.index + 0
df_carrier_1_pathID = df_carrier_1_pathID.drop(columns='index')

#df_carrier_1_pathID_df = pd.DataFrame([df_carrier_1_pathID])
combine_final_req = pd.concat([df_carrier_0_pathID, df_carrier_1_pathID], ignore_index=True)

#combine_final_req = df_carrier_0_pathID.append(df_carrier_1_pathID) #old
combine_final_req.to_csv("DCP1_Requests.csv", index=False)

#Appending Requests Before Disaster and After Disaster

app_req = pd.read_csv("DCP1_Requests.csv")

map_req = pd.read_csv("DCP1_Requests.csv")

#map_req = pd.read_csv("Combined_req_01.csv")
fh = open('carrier_0_00_01.request.txt', 'w')
fh1 = open('carrier_1_00_01.request.txt', 'w')
x = 0
y = 0

weight = 10
ese = 0
calculated_priority = 1
for i in range(len(map_req)):
    src = map_req.iloc[i, 0]
    dest = map_req.iloc[i, 1]
    Min_request_bw = map_req.iloc[i, 4]
    Max_request_bw = map_req.iloc[i, 5]
    ub_bw = math.ceil(Max_request_bw/1000)
    carrier_id = map_req.iloc[i, 2]
    #calculated_priority = map_req.iloc[i, 13]
    num_paths = map_req.iloc[i, 9]
    cus_car = map_req.iloc[i, 14]
    path_ID = map_req.iloc[i, 15]
    if carrier_id == 0:
        #fh.write('path-id:', j, ' s=', src, ' d=', dest, ' customer/carrier_id=', 2, ' weight=', 10, ' is_esen=', 1, ' amount=', ub_bw)
        fh.write('path-id:%d s=%d d=%d customer/carrier_id=%d weight=%d is_esen=%d Max_bandwidth=%d Min_bandwidth=%d Priority=%d\n' % (path_ID, src, dest, cus_car, weight, ese, Max_request_bw, Max_request_bw, calculated_priority))
        x = x + 1
    elif carrier_id == 1:
        #fh1.write('path-id:', j, ' s=', src, ' d=', dest, ' customer/carrier_id=', 2, ' weight=', 10, ' is_esen=', 1, ' amount=', ub_bw)
        fh1.write('path-id:%d s=%d d=%d customer/carrier_id=%d weight=%d is_esen=%d Max_bandwidth=%d Min_bandwidth=%d Priority=%d\n' % (path_ID, src, dest, cus_car, weight, ese, Max_request_bw, Max_request_bw, calculated_priority))
        #fh1.write('path-id:%d s=%d d=%d customer/carrier_id=%d weight=%d is_esen=%d amount=%d' % y % src % dest % cus_car % weight % ese % ub_bw)
        y = y + 1





fh.close()
fh1.close()
src = open("carrier_0_00_01.request.txt", "r")
#fline = "Path#=%d Customer_Carrier#=3\n", % (x) # Prepending string
oline = src.readlines()
oline.insert(0, "Path#=%d Customer_Carrier#=3\n" % x)
src.close()
src = open("carrier_0_00_01.request.txt", "w")
src.writelines(oline)
src.close()

src = open("carrier_1_00_01.request.txt", "r")
oline = src.readlines()
oline.insert(0, "Path#=%d Customer_Carrier#=3\n" % y)
src.close()
src = open("carrier_1_00_01.request.txt", "w")
src.writelines(oline)
src.close()
