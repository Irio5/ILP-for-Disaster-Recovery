import networkx as nx
import pandas as pd
import matplotlib.pyplot as plt
import math
import random
pd.options.mode.chained_assignment = None


links = pd.read_csv("links_DCP2_AD.csv")   #read links file
nodes = pd.read_csv("nodes_DCP2_AD.csv")   #read nodes file
list_nodes = nodes['ID'].tolist()


"""
DCP2 Set2 Creation
"""
Set2_data = {'Src_node': [9999], 'Dst_node': [9999], 'Carrier_ID': [9999], 'Users_count': [9999], 'Min_Required_Lightpath_Bandwidth': [9999], 'Max_Required_Lightpath_Bandwidth': [9999], 'Priority_1': [9999], 'Priority_2': [9999], 'Priority_3': [9999]}
Set2_df = pd.DataFrame(Set2_data)
rows_count = Set2_df.shape[0]
Set2_df = Set2_df.drop(Set2_df.index[range(rows_count)])

dcp_2_set_1 = pd.read_csv("DCP2_Set1.csv")

choosing_node_list = [-2, -1, 0, 1, 2]
#carrier_set = [0, 1]
#choosing_user_list =

for k in range(0, 50):
    rows_count = Set2_df.shape[0]
    Set2_df = Set2_df.drop(Set2_df.index[range(rows_count)])
    for i in range(len(dcp_2_set_1)):
        set1_src_nodes = dcp_2_set_1['Src_node'].tolist()
        set1_dst_nodes = dcp_2_set_1['Dst_node'].tolist()

        set1_users_count = dcp_2_set_1['Users_count'].tolist()
        set1_min_BW = dcp_2_set_1['Min_Required_Lightpath_Bandwidth'].tolist()
        set1_max_BW = dcp_2_set_1['Max_Required_Lightpath_Bandwidth'].tolist()

        choosing_user = random.randint(-200, 600)
        choosing_BW = random.randint(-10000, 40000)
        # choosing_node_list = [-2, -1, 0, 1, 2]
        # carrier_set = [0, 1]

        for j in range(0, 50):
            src_node = set1_src_nodes[i] + random.choice(choosing_node_list)
            if src_node in list_nodes:
                break

        for k in range(0, 60):
            dst_node = set1_dst_nodes[i] + random.choice(choosing_node_list)
            if (dst_node in list_nodes) & (dst_node != src_node):
                break

        users = set1_users_count[i] + choosing_user
        Min_BW = set1_min_BW[i] + choosing_BW
        Max_BW = set1_max_BW[i] + choosing_BW
        prior_req = math.ceil(users * 0.33)

        row = {'Src_node': src_node, 'Dst_node': dst_node, 'Carrier_ID': 9999, 'Users_count': users,
               'Min_Required_Lightpath_Bandwidth': Min_BW, 'Max_Required_Lightpath_Bandwidth': Max_BW,
               'Priority_1': prior_req, 'Priority_2': prior_req, 'Priority_3': prior_req}
        #Set2_df = Set2_df.append(row, ignore_index=True)

        row_todf = pd.DataFrame([row])
        Set2_df = pd.concat([Set2_df, row_todf], ignore_index=True)

    common_connections = Set2_df[Set2_df.duplicated(['Src_node', 'Dst_node', 'Carrier_ID'])]
    if len(common_connections) == 0:
        print("No common connection", len(common_connections))
        break



#Set2_df.to_csv("DCP2_Set2_lightpathReq.csv", index=False)

"""
Designing carriers
"""
carriers_list = [0, 1, 2]
carriers_bible = pd.read_csv("Price_lists.csv")

result_df = Set2_df.loc[(Set2_df['Max_Required_Lightpath_Bandwidth'] > 10) & (Set2_df['Max_Required_Lightpath_Bandwidth'] < 500000)]
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
    temp_price_3 = 99999
    temp_flat_1 = 0
    temp_flat_2 = 0
    temp_flat_3 = 99999
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
        elif (carriers_bible.iloc[j, 3] == 2) & (carriers_bible.iloc[j, 1] == s_node) & (carriers_bible.iloc[j, 2] == d_node):
            print("success  3")
            temp_price_3 = carriers_bible.iloc[j, 4]
            temp_flat_3 = carriers_bible.iloc[j, 5]

    if (temp_price_1 > temp_price_2) & (temp_price_3 > temp_price_2):
        print('lowest price from carrier 1 = ', temp_price_2)
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = 1
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_2

    elif (temp_price_1 > temp_price_3) & (temp_price_2 > temp_price_3):
        print('lowest price from carrier 2 = ', temp_price_3)
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = 2
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_3

    elif (temp_price_2 > temp_price_1) & (temp_price_3 > temp_price_1):
        print('lowest price from carrier 0 = ', temp_price_1)
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = 0
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_1

    elif (temp_price_1 == temp_price_2) & (temp_price_3 > temp_price_2):
        print('lowest price from carrier 0 or 1 = ', temp_price_2)
        list_car = [1, 0]
        c_id = random.choice(list_car)
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = c_id
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_2

    elif (temp_price_1 == temp_price_3) & (temp_price_2 > temp_price_1):
        print('lowest price from carrier 0 or 2 = ', temp_price_3)
        list_car = [2, 0]
        c_id = random.choice(list_car)
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = c_id
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_3

    elif (temp_price_2 == temp_price_3) & (temp_price_1 > temp_price_2):
        print('lowest price from carrier 1 or 2 = ', temp_price_2)
        list_car = [1, 2]
        c_id = random.choice(list_car)
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = c_id
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_2

    elif (temp_price_1 == temp_price_2) & (temp_price_2 == temp_price_3):
        print('lowest price from carrier 0 or 1 or 2= ', temp_price_2)
        list_car = [2, 1, 0]
        c_id = random.choice(list_car)
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Carrier_ID'] = c_id
        result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Payment'] = temp_price_2


    else:
        print('Carrier ID not assigned')


    result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node), 'Number of lightpaths'] = num_lightpaths
    result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node) & (result_df['Carrier_ID'] == 0), 'Flat_Payment'] = temp_flat_1 * num_lightpaths
    result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node) & (result_df['Carrier_ID'] == 1), 'Flat_Payment'] = temp_flat_2 * num_lightpaths
    result_df.loc[(result_df['Src_node'] == s_node) & (result_df['Dst_node'] == d_node) & (result_df['Carrier_ID'] == 2), 'Flat_Payment'] = temp_flat_3 * num_lightpaths


print(result_df)
final_result_df = result_df.sort_values(by=['Request_Priority'], ascending=False)
final_result_df.reset_index(inplace=True)
final_result_df['Final_Priority'] = final_result_df.index + 1
final_result_df = final_result_df.drop(columns='index')
print(final_result_df)

final_result_df.to_csv("Pre_Final_LightpathReq_DCP2_Set2.csv", index=False)

"""
Resetting Priorities for both the Carriers
"""
reset_prior_df = pd.read_csv("Pre_Final_LightpathReq_DCP2_Set2.csv")
df_carrier_0 = reset_prior_df
df_carrier_1 = reset_prior_df
df_carrier_2 = reset_prior_df
for i in range(len(reset_prior_df)):

    #carrier_id = reset_prior_df.iloc[i, 2]
    #calculated_priority = reset_prior_df.iloc[i, 13]
    #result_df = lightpath_req.loc[(lightpath_req['Max_Required_Lightpath_Bandwidth'] > 10) & (
    #           lightpath_req['Max_Required_Lightpath_Bandwidth'] < 500000)]
    df_carrier_0 = reset_prior_df.loc[reset_prior_df['Carrier_ID'] == 0]
    df_carrier_1 = reset_prior_df.loc[reset_prior_df['Carrier_ID'] == 1]
    df_carrier_2 = reset_prior_df.loc[reset_prior_df['Carrier_ID'] == 2]

df_carrier_0.reset_index(inplace=True)
#df_carrier_0['Final_Priority'] = 1
#df_carrier_0['Path_ID'] = df_carrier_0.index
df_carrier_0 = df_carrier_0.drop(columns='index')
calculated_priority_list = [1, 2, 3]

for i in range(len(df_carrier_0)):
    #max_bw = df_carrier_0.iloc[i, 5]
    #df_carrier_0.iloc[i, 4] = math.ceil(max_bw * 0.9)

    calculated_priority_new = random.choice(calculated_priority_list)
    max_bw = df_carrier_0.iloc[i, 5]
    df_carrier_0.iloc[i, 12] = calculated_priority_new
    df_carrier_0.iloc[i, 13] = calculated_priority_new
    if calculated_priority_new == 1:
        df_carrier_0.iloc[i, 4] = math.ceil(max_bw * 0.9)
    else:
        df_carrier_0.iloc[i, 4] = math.ceil(max_bw * 0.8)


df_carrier_1.reset_index(inplace=True)
#df_carrier_1['Final_Priority'] = 1
#df_carrier_1['Path_ID'] = df_carrier_1.index
df_carrier_1 = df_carrier_1.drop(columns='index')
calculated_priority_list = [1, 2, 3]

for i in range(len(df_carrier_1)):
    #max_bw = df_carrier_1.iloc[i, 5]
    #df_carrier_1.iloc[i, 4] = math.ceil(max_bw * 0.9)
    calculated_priority_new = random.choice(calculated_priority_list)
    max_bw = df_carrier_1.iloc[i, 5]
    df_carrier_1.iloc[i, 12] = calculated_priority_new
    df_carrier_1.iloc[i, 13] = calculated_priority_new
    if calculated_priority_new == 1:
        df_carrier_1.iloc[i, 4] = math.ceil(max_bw * 0.9)
    else:
        df_carrier_1.iloc[i, 4] = math.ceil(max_bw * 0.8)

#Additaion
df_carrier_2.reset_index(inplace=True)

#df_carrier_1['Path_ID'] = df_carrier_1.index
df_carrier_2 = df_carrier_2.drop(columns='index')
calculated_priority_new_list = [2, 3, 4]
for i in range(len(df_carrier_2)):
    calculated_priority_new = random.choice(calculated_priority_new_list)
    max_bw = df_carrier_2.iloc[i, 5]
    df_carrier_2.iloc[i, 12] = calculated_priority_new
    df_carrier_2.iloc[i, 13] = calculated_priority_new
    if calculated_priority_new == 2:
        df_carrier_2.iloc[i, 4] = math.ceil(max_bw * 0.8)
    else:
        df_carrier_2.iloc[i, 4] = math.ceil(max_bw * 0.7)



#Addition end

combine_final_req = pd.concat([df_carrier_0, df_carrier_1, df_carrier_2], ignore_index=True)
combine_final_req.to_csv("DCP2_Set2.csv", index=False)

print("this is carrier 0")
print(df_carrier_0)
print("this is carrier 1")
print(df_carrier_1)
print("this is carrier 2")
print(df_carrier_2)
"""
End of Reseting Priorities for both the Carriers
"""

dcp_2_set_2 = pd.read_csv("DCP2_Set2.csv")

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
    dcp_2_set_2 = pd.concat([dcp_2_set_2, direct_row_todf], ignore_index=True)



sum_columns = ['Users_count', 'Min_Required_Lightpath_Bandwidth', 'Max_Required_Lightpath_Bandwidth', 'Priority_1',	'Priority_2', 'Priority_3',	'Number of lightpaths',	'Payment', 'Flat_Payment']


# Find duplicate rows based on Src_node, Dst_node, and Carrier_ID
duplicates = dcp_2_set_2.duplicated(subset=['Src_node', 'Dst_node', 'Carrier_ID'], keep=False)

# Separate duplicate and unique rows
df_duplicates = dcp_2_set_2[duplicates]
df_unique = dcp_2_set_2[~duplicates]

# Merge duplicate rows by summing the required columns
dcp_2_set_2_merged = df_duplicates.groupby(['Src_node', 'Dst_node', 'Carrier_ID'], as_index=False).agg(
    {col: 'sum' for col in sum_columns} |  # Sum required columns
    {'Request_Priority': lambda x: 1, 'Final_Priority': lambda x: 1,}  # Set these to 1
)

# Combine back with unique rows
df_final = pd.concat([df_unique, dcp_2_set_2_merged], ignore_index=True)

df_final['From_DCP'] = 4

reset_path_ID_df = df_final
for i in range(len(reset_path_ID_df)):

    #carrier_id = reset_prior_df.iloc[i, 2]
    #calculated_priority = reset_prior_df.iloc[i, 13]
    #result_df = lightpath_req.loc[(lightpath_req['Max_Required_Lightpath_Bandwidth'] > 10) & (
    #           lightpath_req['Max_Required_Lightpath_Bandwidth'] < 500000)]
    df_carrier_0_pathID = reset_path_ID_df.loc[reset_path_ID_df['Carrier_ID'] == 0]
    df_carrier_1_pathID = reset_path_ID_df.loc[reset_path_ID_df['Carrier_ID'] == 1]
    df_carrier_2_pathID = reset_path_ID_df.loc[reset_path_ID_df['Carrier_ID'] == 2]

df_carrier_0_pathID.reset_index(inplace=True)
df_carrier_0_pathID['Path_ID'] = df_carrier_0_pathID.index + 0
df_carrier_0_pathID = df_carrier_0_pathID.drop(columns='index')


df_carrier_1_pathID.reset_index(inplace=True)
df_carrier_1_pathID['Path_ID'] = df_carrier_1_pathID.index + 0
df_carrier_1_pathID = df_carrier_1_pathID.drop(columns='index')

df_carrier_2_pathID.reset_index(inplace=True)
df_carrier_2_pathID['Path_ID'] = df_carrier_2_pathID.index + 0
df_carrier_2_pathID = df_carrier_2_pathID.drop(columns='index')

#df_carrier_1_pathID_df = pd.DataFrame([df_carrier_1_pathID])
combine_final_req1 = pd.concat([df_carrier_0_pathID, df_carrier_1_pathID], ignore_index=True)
combine_final_req = pd.concat([combine_final_req1, df_carrier_2_pathID], ignore_index=True)

#combine_final_req = df_carrier_0_pathID.append(df_carrier_1_pathID) #old
combine_final_req.to_csv("DCP2_Requests_2.csv", index=False)






















