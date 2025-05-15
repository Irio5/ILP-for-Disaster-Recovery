import networkx as nx
import pandas as pd
import matplotlib.pyplot as plt
import math
import numpy as np
pd.options.mode.chained_assignment = None  # default='warn' #to remove the SettingWithCopyWarning:

#Original_requests = pd.read_csv("DCP_Requests.csv")
Users_carrier_0_links = pd.read_csv("ksp_carrier0_Final.csv")   #read links file
Users_carrier_0_links['Used_F_Links'] = Users_carrier_0_links['Used_F_Links'].fillna("0->0")
Users_carrier_1_links = pd.read_csv("ksp_carrier1_Final.csv")   #read links file
Users_carrier_1_links['Used_F_Links'] = Users_carrier_1_links['Used_F_Links'].fillna("0->0")
#Users_carrier_2_links = pd.read_csv("carrier2_Final.csv")   #read links file

Users_carrier_0_links["Carrier_ID"] = 0
Users_carrier_1_links["Carrier_ID"] = 1
#Users_carrier_2_links["Carrier_ID"] = 2

frames = [Users_carrier_0_links, Users_carrier_1_links]
final_result = pd.concat(frames)
#df_filtered = result[(result['Result'] == 1) & (result['Customer_ID'] == 2)]

#print(final_result)
final_result.to_csv("Combined_final_result.csv", index=False)

dcp2_final_result = final_result[final_result['Customer_ID'] == 4]
dcp2_final_result.to_csv("DCP2_final_result.csv", index=False)

#dcp2_final_result = final_result[(final_result['DCP2_Result'] == 1) & (final_result['Customer_ID'] == 2)]


"""
Users Addition
"""
"""
final_result['Users_count'] = 0
for i in range(len(Original_requests)):
    s_node = Original_requests.iloc[i, 0]
    d_node = Original_requests.iloc[i, 1]
    carrier_id = Original_requests.iloc[i, 2]

    users_count = Original_requests.iloc[i, 3]
    for j in range(len(final_result)):
        if (final_result.iloc[j, 1] == s_node) & (final_result.iloc[j, 2] == d_node) & (final_result.iloc[j, 11] == carrier_id):
            final_result.iloc[j, 12] = users_count


final_result.to_csv("Final_Users_added_result.csv", index=False)
"""

"""
Checking IP Restored
"""
users_total_requested = dcp2_final_result['Users_count'].sum()
ip_restored = 15000 - users_total_requested



#user_final_results = pd.read_csv("Final_Users_added_result_DCP2.csv")

"""
create final results Dataframe
"""
#final_data = {'Imidiate_Restored_Service': [9999], 'Nonimidiate_Restored_Service': [9999], 'Regular_Imidiate_payement': [9999], 'Regular_Nonimidiate_payement': [9999], 'Regular_Total_payment': [9999], 'Towards_Carrier_0': [9999], 'Towards_Carrier_1': [9999], 'Total_Advertised_payement': [9999]}
final_data = {'IRS': [9999], 'NIRS_1': [9999], 'NIRS_2': [9999], 'NIRS_3': [9999], 'NIRS_4': [9999], 'NIRS_5': [9999], 'Reject' : [9999], 'Regular_IP': [9999], 'Regular_NIRS_1': [9999], 'Regular_NIRS_2': [9999], 'Regular_NIRS_3': [9999], 'Regular_NIRS_4': [9999], 'Regular_NIRS_5': [9999], 'Regular_Total_payment': [9999], 'Towards_Carrier_0': [9999], 'Towards_Carrier_1': [9999], 'Towards_Carrier_2': [9999], 'Total_Advertised_payment': [9999]}
result_df = pd.DataFrame(final_data)
rows_count_now = result_df.shape[0]
result_df = result_df.drop(result_df.index[range(rows_count_now)])


"""
Result Analysis
"""


for i in range(len(dcp2_final_result)):
    carrier_id = dcp2_final_result.iloc[i, 20]
    satisfied_BW = dcp2_final_result.iloc[i, 5]
    result = dcp2_final_result.iloc[i, 7]
    recovery_time = dcp2_final_result.iloc[i, 8]
    user_count = dcp2_final_result.iloc[i, 12]
    payment = dcp2_final_result.iloc[i, 13]
    if carrier_id == 0:
        if result == 1:
            row_data = {'IRS': user_count, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': payment, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': payment, 'Towards_Carrier_1': 0, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
            #result_df = result_df.append(row_data, ignore_index=True) #old
        elif (result == 2) & (recovery_time == 1):
            row_data = {'IRS': 0, 'NIRS_1': user_count, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': payment, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': payment, 'Towards_Carrier_1': 0, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time == 2):
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': user_count, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': payment, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': payment, 'Towards_Carrier_1': 0, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time == 3):
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': user_count, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': payment, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': payment, 'Towards_Carrier_1': 0, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time == 4):
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': user_count, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': payment, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': payment, 'Towards_Carrier_1': 0, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time >= 5):
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': user_count, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': payment, 'Regular_Total_payment': payment, 'Towards_Carrier_0': payment, 'Towards_Carrier_1': 0, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif result == 0:
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': user_count, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': 0, 'Towards_Carrier_0': 0, 'Towards_Carrier_1': 0, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': 0}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)

    elif carrier_id == 1:
        if result == 1:
            row_data = {'IRS': user_count, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': payment, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': 0, 'Towards_Carrier_1': payment, 'Towards_Carrier_2': 0,  'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time == 1):
            row_data = {'IRS': 0, 'NIRS_1': user_count, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': payment, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': 0, 'Towards_Carrier_1': payment, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time == 2):
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': user_count, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': payment, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': 0, 'Towards_Carrier_1': payment, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time == 3):
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': user_count, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': payment, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': 0, 'Towards_Carrier_1': payment, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time == 4):
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': user_count, 'NIRS_5': 0, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': payment, 'Regular_NIRS_5': 0, 'Regular_Total_payment': payment, 'Towards_Carrier_0': 0, 'Towards_Carrier_1': payment, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif (result == 2) & (recovery_time >= 5):
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': user_count, 'Reject': 0, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': payment, 'Regular_Total_payment': payment, 'Towards_Carrier_0': 0, 'Towards_Carrier_1': payment, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': payment}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)
        elif result == 0:
            row_data = {'IRS': 0, 'NIRS_1': 0, 'NIRS_2': 0, 'NIRS_3': 0, 'NIRS_4': 0, 'NIRS_5': 0, 'Reject': user_count, 'Regular_IP': 0, 'Regular_NIRS_1': 0, 'Regular_NIRS_2': 0, 'Regular_NIRS_3': 0, 'Regular_NIRS_4': 0, 'Regular_NIRS_5': 0, 'Regular_Total_payment': 0, 'Towards_Carrier_0': 0, 'Towards_Carrier_1': 0, 'Towards_Carrier_2': 0, 'Total_Advertised_payment': 0}
            row_data_df = pd.DataFrame([row_data])
            result_df = pd.concat([result_df, row_data_df], ignore_index=True)



result_df.to_csv("DCP2_Final_Results_Allconnections.csv", index=False)

"""
create final results Dataframe
"""""

merged_data = {'Ip_res': [9999], 'IRS': [9999], 'NIRS_1': [9999], 'NIRS_2': [9999], 'NIRS_3': [9999], 'NIRS_4': [9999], 'NIRS_5': [9999], 'Reject': [9999], 'Regular_IP': [9999], 'Regular_NIRS_1': [9999], 'Regular_NIRS_2': [9999], 'Regular_NIRS_3': [9999], 'Regular_NIRS_4': [9999], 'Regular_NIRS_5': [9999], 'Regular_Total_payment': [9999], 'Towards_Carrier_0': [9999], 'Towards_Carrier_1': [9999], 'Towards_Carrier_2': [9999], 'Total_Advertised_payment': [9999], 'Total_Final_Price': [9999], 'Priority_1_restored': [9999], 'Total_Satisfied_BW': [9999]}


merged_data_df = pd.DataFrame(merged_data)
rows_count_now1 = merged_data_df.shape[0]
merged_data_df = merged_data_df.drop(merged_data_df.index[range(rows_count_now1)])



IRS = result_df['IRS'].sum()
NIRS_1 = result_df['NIRS_1'].sum()
NIRS_2 = result_df['NIRS_2'].sum()
NIRS_3 = result_df['NIRS_3'].sum()
NIRS_4 = result_df['NIRS_4'].sum()
NIRS_5 = result_df['NIRS_5'].sum()
REJECT = result_df['Reject'].sum()

RIP = result_df['Regular_IP'].sum()
RNIP_1 = result_df['Regular_NIRS_1'].sum()
RNIP_2 = result_df['Regular_NIRS_2'].sum()
RNIP_3 = result_df['Regular_NIRS_3'].sum()
RNIP_4 = result_df['Regular_NIRS_4'].sum()
RNIP_5 = result_df['Regular_NIRS_5'].sum()

RTP = result_df['Regular_Total_payment'].sum()
TC0 = result_df['Towards_Carrier_0'].sum()
TC1 = result_df['Towards_Carrier_1'].sum()
TC2 = result_df['Towards_Carrier_2'].sum()
TAP = result_df['Total_Advertised_payment'].sum()

"Price Calculation"
# Find max Lead_Time for Carrier_ID 0
max_lead_time_0 = dcp2_final_result[dcp2_final_result['Carrier_ID'] == 0]['Lead_Time'].max()

# Find max Lead_Time for Carrier_ID 1
max_lead_time_1 = dcp2_final_result[dcp2_final_result['Carrier_ID'] == 1]['Lead_Time'].max()

# Add both max values
total_link_recovery = max_lead_time_0 + max_lead_time_1
total_survived_links_used = dcp2_final_result['S_Links'].sum()
total_satisfied_BW = dcp2_final_result['Satisfied_Bandwidth'].sum()
total_price = (total_link_recovery*50) + (total_survived_links_used*2)

"Priority 1 Calculation"
total_priority_1 = 0
for i in range(len(dcp2_final_result)):
    priority = dcp2_final_result.iloc[i, 11]
    result = dcp2_final_result.iloc[i, 7]
    if (result != 0) & (priority == 1):
        total_priority_1 = total_priority_1 + 1





final_IRS = 15000 - (NIRS_1 + NIRS_2 + NIRS_3 + NIRS_4 + NIRS_5 + REJECT)

this_data = {'Ip_res': ip_restored, 'IRS': final_IRS, 'NIRS_1': NIRS_1, 'NIRS_2': NIRS_2, 'NIRS_3': NIRS_3, 'NIRS_4': NIRS_4, 'NIRS_5': NIRS_5, 'Reject': REJECT, 'Regular_IP': RIP, 'Regular_NIRS_1': RNIP_1, 'Regular_NIRS_2': RNIP_2, 'Regular_NIRS_3': RNIP_3,
               'Regular_NIRS_4': RNIP_4, 'Regular_NIRS_5': RNIP_5, 'Regular_Total_payment': RTP, 'Towards_Carrier_0': TC0,
               'Towards_Carrier_1': TC1, 'Towards_Carrier_2': TC2, 'Total_Advertised_payment': TAP, 'Total_Final_Price': total_price, 'Priority_1_restored': total_priority_1, 'Total_Satisfied_BW': total_satisfied_BW}
this_data_df = pd.DataFrame([this_data])
merged_data_df = pd.concat([merged_data_df, this_data_df], ignore_index=True)

#merged_data_df = merged_data_df.append(this_data, ignore_index=True) #old




merged_data_df.to_csv("DCP2_Analyzed_Results.csv", index=False)
