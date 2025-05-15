#from cplex import Cplex, SparsePair
#from cplex.exceptions import CplexError
import csv
import networkx as nx
import pandas as pd
import matplotlib.pyplot as plt
import random

pd.options.mode.chained_assignment = None

"""
Generating user requests
"""
print("Content requests generation for DCP2..... Starts")

request_data = {'ID': [9999], 'contents': [9999], 'Required_BW': [9999], 'Source': [9999], 'Destinations': [9999]}
request_df = pd.DataFrame(request_data)
list1 = [20, 21, 26, 27, 28, 29] #1,5,7, DCs are damaged
#list2 = [2, 3, 4, 11, 12, 13] #2,5,6 DCs are damaged

rows_count_req = request_df.shape[0]
request_df = request_df.drop(request_df.index[range(rows_count_req)])

for i in range(15000):
    ID = i
    contents = random.choice(list1)
    Required_BW = random.randint(50, 80)

    add_row = {'ID': ID, 'contents': contents, 'Required_BW': Required_BW, 'Source': 9999, 'Destinations': 9999}
    add_row_df = pd.DataFrame([add_row])
    request_df = pd.concat([request_df, add_row_df], ignore_index=True)

request_df.to_csv("Content_User_requests.csv", index=False)

print("Content requests generation for DCP2..... Done!!!! ")

"""
Designing contents
"""
content_list = [20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35]
contents = pd.read_csv("contents_primary_DCP2.csv")

"""
Reading user requests
"""
requests_df = pd.read_csv("Content_User_requests.csv")
list_user_id = requests_df['ID'].tolist()
user_id_arr = [int(item) for item in list_user_id]

for n in user_id_arr:
    src_dc = \
    contents.loc[contents['ID'] == (requests_df.loc[requests_df['ID'] == n, 'contents'].iloc[0]), 'Primary_DC'].iloc[0]
    dst_dc = \
    contents.loc[contents['ID'] == (requests_df.loc[requests_df['ID'] == n, 'contents'].iloc[0]), 'Backup_DC'].iloc[0]
    requests_df.loc[(requests_df['ID'] == n), 'Source'] = src_dc
    requests_df.loc[(requests_df['ID'] == n), 'Destinations'] = dst_dc

requests_df.to_csv("Final_content_requests.csv", index=False)
