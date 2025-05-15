import os
import glob
import pandas as pd
import math




#path = "~/ESEN-DCI/Data_Accumulation/"

all_files = glob.glob(os.path.join("*.Analyzed_Results.csv"))
df_from_each_file = (pd.read_csv(f, sep=',') for f in all_files)
Average_df = pd.concat(df_from_each_file, ignore_index=True)
Average_df.to_csv("Final_result.csv", index=False)

agg = pd.read_csv("Final_result.csv")


"""
My Addition
"""
final_data = {'Ip_res': [9999], 'IRS': [9999], 'NIRS_1': [9999], 'NIRS_2': [9999], 'NIRS_3': [9999], 'NIRS_4': [9999], 'NIRS_5': [9999],'Regular_IP': [9999], 'Regular_NIRS_1': [9999], 'Regular_NIRS_2': [9999], 'Regular_NIRS_3': [9999], 'Regular_NIRS_4': [9999], 'Regular_NIRS_5': [9999], 'Regular_Total_payment': [9999], 'Towards_Carrier_0': [9999], 'Towards_Carrier_1': [9999], 'Towards_Carrier_2': [9999], 'Total_Advertised_payment': [9999]}
final_df = pd.DataFrame(final_data)
rows_count_now = final_df.shape[0]
final_df = final_df.drop(final_df.index[range(rows_count_now)])

Ip_res = math.ceil(Average_df['Ip_res'].mean())
IRS = math.ceil(Average_df['IRS'].mean())
NIRS_1 = math.ceil(Average_df['NIRS_1'].mean())
NIRS_2 = math.ceil(Average_df['NIRS_2'].mean())
NIRS_3 = math.ceil(Average_df['NIRS_3'].mean())
NIRS_4 = math.ceil(Average_df['NIRS_4'].mean())
NIRS_5 = math.ceil(Average_df['NIRS_5'].mean())

RIP = math.ceil(Average_df['Regular_IP'].mean())
RNIP_1 = math.ceil(Average_df['Regular_NIRS_1'].mean())
RNIP_2 = math.ceil(Average_df['Regular_NIRS_2'].mean())
RNIP_3 = math.ceil(Average_df['Regular_NIRS_3'].mean())
RNIP_4 = math.ceil(Average_df['Regular_NIRS_4'].mean())
RNIP_5 = math.ceil(Average_df['Regular_NIRS_5'].mean())

RTP = math.ceil(Average_df['Regular_Total_payment'].mean())
TC0 = math.ceil(Average_df['Towards_Carrier_0'].mean())
TC1 = math.ceil(Average_df['Towards_Carrier_1'].mean())
TC2 = math.ceil(Average_df['Towards_Carrier_2'].mean())
TAP = math.ceil(Average_df['Total_Advertised_payment'].mean())


this_data = {'Ip_res': Ip_res, 'IRS': IRS, 'NIRS_1': NIRS_1, 'NIRS_2': NIRS_2, 'NIRS_3': NIRS_3, 'NIRS_4': NIRS_4, 'NIRS_5': NIRS_5,
               'Regular_IP': RIP, 'Regular_NIRS_1': RNIP_1, 'Regular_NIRS_2': RNIP_2, 'Regular_NIRS_3': RNIP_3,
               'Regular_NIRS_4': RNIP_4, 'Regular_NIRS_5': RNIP_5, 'Regular_Total_payment': RTP, 'Towards_Carrier_0': TC0,
               'Towards_Carrier_1': TC1, 'Towards_Carrier_2': TC2, 'Total_Advertised_payment': TAP}

"""
My Addition
"""
this_data_df = pd.DataFrame([this_data])
final_df = pd.concat([final_df, this_data_df], ignore_index=True)


#final_df = final_df.append(this_data, ignore_index=True) #old

final_df.to_csv("Aggregated_result.csv", index=False)
