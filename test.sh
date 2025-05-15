cd
HOME=`pwd`


temp_dir="$HOME/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/$case$cost$phase"
temp_dir_p3="$HOME/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/$case$cost" 
temp_dir_numer="$HOME/Ignazio_ECOC_2025/Numerical_Analysis/"
temp_dir_base="$Home/Ignazio_ECOC_2025/"


cd "$temp_dir_numer"
for instance in */
do
  temp5="$(cut -d'_' -f2 <<<"$instance")"
  
  temp6="$(cut -d'/' -f1 <<<"$temp5")"
  echo "Working with Instance Number"
  echo "$temp6"
  #if [ "$temp6" = "$temp3" ]
  if [ "$temp6" -ge 50 ] && [ "$temp6" -le 99 ];
    then
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP1_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP1_Requests_2.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP2_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP2_Requests_2.csv
  
  fi

  cd "$temp_dir_numer"

  
done




cd "$HOME/Ignazio_ECOC_2025/"
#chmod +x 2025_Globecom_Extension_data_Script_final.sh
#./2025_Globecom_Extension_data_Script_final.sh            
             
                
                
                
                
                
                
                
                
                
                
