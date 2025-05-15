cd
HOME=`pwd`

#from 0 to 89
temp_dir_numer="$HOME/Ignazio_ECOC_2025_0418/Numerical_Analysis/"             
cd "$temp_dir_numer"
#x="$0"
for instance in */
do
  #temp5="$(cut -d'_' -f2 <<<"$instance")"
  #temp6="$(cut -d'/' -f1 <<<"$temp5")"
  #temp9="$(cut -d'/' -f1 <<<"$instance")"
 
  temp5=`echo $instance | cut -d'_' -f2`
  temp6=`echo $temp5 | cut -d'/' -f1`
  temp9=`echo $instance | cut -d'/' -f1`
                     
  if [ $temp6 -ge 50 ] && [ $temp6 -le 99 ];
  then
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv
  fi
  #x="$x + 1"
done

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP"
python3 Result_acc_DCP1_ILP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/DCP1_Final_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case0_5_5/ILP/DCP1_Final_result_ILP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/DCP1_Aggregated_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case0_5_5/ILP/DCP1_Aggregated_result_ILP.csv
rm DCP1_Final_result_ILP.csv
rm DCP1_Aggregated_result_ILP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP"
python3 Result_acc_DCP1_KSP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/DCP1_Final_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case0_5_5/KSP/DCP1_Final_result_KSP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/DCP1_Aggregated_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case0_5_5/KSP/DCP1_Aggregated_result_KSP.csv
rm DCP1_Final_result_KSP.csv
rm DCP1_Aggregated_result_KSP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP"
python3 Result_acc_DCP2_ILP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/DCP2_Final_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case0_5_5/ILP/DCP2_Final_result_ILP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/DCP2_Aggregated_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case0_5_5/ILP/DCP2_Aggregated_result_ILP.csv
rm DCP2_Final_result_ILP.csv
rm DCP2_Aggregated_result_ILP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP"
python3 Result_acc_DCP2_KSP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/DCP2_Final_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case0_5_5/KSP/DCP2_Final_result_KSP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/DCP2_Aggregated_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case0_5_5/KSP/DCP2_Aggregated_result_KSP.csv
rm DCP2_Final_result_KSP.csv
rm DCP2_Aggregated_result_KSP.csv




cd "$temp_dir_numer"
#x="$0"
for instance in */
do
  #temp5="$(cut -d'_' -f2 <<<"$instance")"
  #temp6="$(cut -d'/' -f1 <<<"$temp5")"
  #temp9="$(cut -d'/' -f1 <<<"$instance")"
 
  temp5=`echo $instance | cut -d'_' -f2`
  temp6=`echo $temp5 | cut -d'/' -f1`
  temp9=`echo $instance | cut -d'/' -f1`
                     
  if [ $temp6 -ge 200 ] && [ $temp6 -le 249 ];
  then
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv
  fi
  #x="$x + 1"
done

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP"
python3 Result_acc_DCP1_ILP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/DCP1_Final_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case1_5_10/ILP/DCP1_Final_result_ILP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/DCP1_Aggregated_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case1_5_10/ILP/DCP1_Aggregated_result_ILP.csv
rm DCP1_Final_result_ILP.csv
rm DCP1_Aggregated_result_ILP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP"
python3 Result_acc_DCP1_KSP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/DCP1_Final_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case1_5_10/KSP/DCP1_Final_result_KSP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/DCP1_Aggregated_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case1_5_10/KSP/DCP1_Aggregated_result_KSP.csv
rm DCP1_Final_result_KSP.csv
rm DCP1_Aggregated_result_KSP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP"
python3 Result_acc_DCP2_ILP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/DCP2_Final_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case1_5_10/ILP/DCP2_Final_result_ILP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/DCP2_Aggregated_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case1_5_10/ILP/DCP2_Aggregated_result_ILP.csv
rm DCP2_Final_result_ILP.csv
rm DCP2_Aggregated_result_ILP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP"
python3 Result_acc_DCP2_KSP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/DCP2_Final_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case1_5_10/KSP/DCP2_Final_result_KSP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/DCP2_Aggregated_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case1_5_10/KSP/DCP2_Aggregated_result_KSP.csv
rm DCP2_Final_result_KSP.csv
rm DCP2_Aggregated_result_KSP.csv




cd "$temp_dir_numer"
#x="$0"
for instance in */
do
  #temp5="$(cut -d'_' -f2 <<<"$instance")"
  #temp6="$(cut -d'/' -f1 <<<"$temp5")"
  #temp9="$(cut -d'/' -f1 <<<"$instance")"
 
  temp5=`echo $instance | cut -d'_' -f2`
  temp6=`echo $temp5 | cut -d'/' -f1`
  temp9=`echo $instance | cut -d'/' -f1`
                     
  if [ $temp6 -ge 350 ] && [ $temp6 -le 399 ];
  then
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv
    cp -f $temp_dir_numer$temp9/DCI_Analysis/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv
  fi
  #x="$x + 1"
done

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP"
python3 Result_acc_DCP1_ILP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/DCP1_Final_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case2_10_10/ILP/DCP1_Final_result_ILP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/ILP/DCP1_Aggregated_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case2_10_10/ILP/DCP1_Aggregated_result_ILP.csv
rm DCP1_Final_result_ILP.csv
rm DCP1_Aggregated_result_ILP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP"
python3 Result_acc_DCP1_KSP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/DCP1_Final_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case2_10_10/KSP/DCP1_Final_result_KSP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP1/KSP/DCP1_Aggregated_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case2_10_10/KSP/DCP1_Aggregated_result_KSP.csv
rm DCP1_Final_result_KSP.csv
rm DCP1_Aggregated_result_KSP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP"
python3 Result_acc_DCP2_ILP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/DCP2_Final_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case2_10_10/ILP/DCP2_Final_result_ILP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/ILP/DCP2_Aggregated_result_ILP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case2_10_10/ILP/DCP2_Aggregated_result_ILP.csv
rm DCP2_Final_result_ILP.csv
rm DCP2_Aggregated_result_ILP.csv

cd "$HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP"
python3 Result_acc_DCP2_KSP.py
rm *.Analyzed_Results.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/DCP2_Final_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case2_10_10/KSP/DCP2_Final_result_KSP.csv
cp -f $HOME/Ignazio_ECOC_2025_0418/Result_Analysis/Results_Aggregation/DCP2/KSP/DCP2_Aggregated_result_KSP.csv $HOME/Ignazio_ECOC_2025_0418/Final_Aggregated_Result/case2_10_10/KSP/DCP2_Aggregated_result_KSP.csv
rm DCP2_Final_result_KSP.csv
rm DCP2_Aggregated_result_KSP.csv
