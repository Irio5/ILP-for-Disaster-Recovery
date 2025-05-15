import networkx as nx
import pandas as pd
import matplotlib.pyplot as plt


links_BD = pd.read_csv("links_DCP1_BD.csv")   #read links file
DCP1_predisaster_topology = {'Src_node': [9999], 'Dst_node': [9999], 'Capacity': [9999], 'Cost': [9999]}
DCP1_predisaster_topology_df = pd.DataFrame(DCP1_predisaster_topology)
rows_count = DCP1_predisaster_topology_df.shape[0]
DCP1_predisaster_topology_df = DCP1_predisaster_topology_df.drop(DCP1_predisaster_topology_df.index[range(rows_count)])

for i in range(len(links_BD)):
    #print(i, result_df.iloc[i, 1])
    s_node = links_BD.iloc[i, 1]
    d_node = links_BD.iloc[i, 2]
    cap = 100000
    cos = 1
    add_row = {'Src_node': s_node, 'Dst_node': d_node, 'Capacity': cap, 'Cost': cos}
    add_row_df = pd.DataFrame([add_row])
    DCP1_predisaster_topology_df = pd.concat([DCP1_predisaster_topology_df, add_row_df], ignore_index=True)

#print(DCP1_predisaster_topology_df)












DCI_carrier_0_links = pd.read_csv("DCI_carrier_0_links.csv")   #read links file
DCI_carrier_1_links = pd.read_csv("DCI_carrier_1_links.csv")   #read links file

DCI_carrier_0_links['Carrier_ID'] = 0
DCI_carrier_1_links['Carrier_ID'] = 1

frames = [DCI_carrier_0_links, DCI_carrier_1_links]
result = pd.concat(frames)
df_filtered = result[(result['Result'] == 1) & (result['Customer_ID'] == 3)]

#print(df_filtered)
df_filtered.to_csv("links_DCP1_AD.csv", index=False)


DCP1_survived_topology = {'Src_node': [9999], 'Dst_node': [9999], 'Capacity': [9999], 'Cost': [9999]}
DCP1_survived_topology_df = pd.DataFrame(DCP1_survived_topology)
rows_count = DCP1_survived_topology_df.shape[0]
DCP1_survived_topology_df = DCP1_survived_topology_df.drop(DCP1_survived_topology_df.index[range(rows_count)])

for i in range(len(df_filtered)):
    #print(i, result_df.iloc[i, 1])
    s_node = df_filtered.iloc[i, 1]
    d_node = df_filtered.iloc[i, 2]
    cap = df_filtered.iloc[i, 5]
    cos = 1
    Carr_ID = df_filtered.iloc[i, 11]
    add_row = {'Src_node': s_node, 'Dst_node': d_node, 'Capacity': cap, 'Cost': cos, 'Carrier_ID': Carr_ID}
    add_row_df = pd.DataFrame([add_row])
    DCP1_survived_topology_df = pd.concat([DCP1_survived_topology_df, add_row_df], ignore_index=True)


# Initialize an empty dataframe to store rows to be added
rows_to_add = []

# Iterate through each row in DCP1_predisaster_topology_df
for index, row in DCP1_predisaster_topology_df.iterrows():
    src_node = row['Src_node']
    dst_node = row['Dst_node']

    # Check if the (Src_node, Dst_node) pair already exists in DCP1_survived_topology_df
    if ((DCP1_survived_topology_df['Src_node'] == src_node) & (
            DCP1_survived_topology_df['Dst_node'] == dst_node)).any():
        continue  # Skip adding this row if it already exists

    # Prepare row to be added
    add_row = {
        'Src_node': src_node,
        'Dst_node': dst_node,
        'Capacity': 0,  # Set your desired default values for Capacity and Cost
        'Cost': 999,
        'Carrier_ID': 999
    }

    rows_to_add.append(add_row)

# Add all rows at once to DCP1_survived_topology_df
if rows_to_add:
    DCP1_survived_topology_df = pd.concat([DCP1_survived_topology_df, pd.DataFrame(rows_to_add)], ignore_index=True)

print(DCP1_survived_topology_df)


DCP1_survived_topology_df.to_csv("DCP1_survived_topology.csv", index=False)



