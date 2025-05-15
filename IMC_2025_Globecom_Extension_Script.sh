cd
HOME=`pwd`

#Phase 0
#Initialize Carrier_0, Carrier_1, PNE (Before disaster) at phase_00 folder


#Phase 1
#Copy all files from phase__00 to phase_01 folder
cp -R ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_00/* ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/



#Include DCP1, DCP2, DCP3, and DCP4 topologies before disaster into DCP1, DCP2, DCP3, and DCP4 folders respectively
cp -f ~/Ignazio_ECOC_2025/DCP1_Heuristic/links_DCP1_BD.csv ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/dcp_1
cp -f ~/Ignazio_ECOC_2025/DCP1_Heuristic/nodes_DCP1_BD.csv ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/dcp_1

cp -f ~/Ignazio_ECOC_2025/DCP2_Heuristic/links_DCP2_BD.csv ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/dcp_2
cp -f ~/Ignazio_ECOC_2025/DCP2_Heuristic/nodes_DCP2_BD.csv ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/dcp_2

#cp -f ~/Ignazio_ECOC_2025/DCP3_Heuristic/links_DCP3_BD.csv ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/dcp_3
#cp -f ~/Ignazio_ECOC_2025/DCP3_Heuristic/nodes_DCP3_BD.csv ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/dcp_3

#cp -f ~/Ignazio_ECOC_2025/DCP4_Heuristic/links_DCP4_BD.csv ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/dcp_4
#cp -f ~/Ignazio_ECOC_2025/DCP4_Heuristic/nodes_DCP4_BD.csv ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/before_disaster/phase_01/dcp_4

#Copy all original requests (before disaster) to cspt folder

cp -f ~/Ignazio_ECOC_2025/Requests/carrier_0.input.request ~/Ignazio_ECOC_2025/cspt/carrier_0.input.request
cp -f ~/Ignazio_ECOC_2025/Requests/carrier_1.input.request ~/Ignazio_ECOC_2025/cspt/carrier_1.input.request
#cp -f ~/Ignazio_ECOC_2025/Requests/carrier_2.input.request ~/Ignazio_ECOC_2025/cspt/carrier_2.input.request

#Phase 2
#generating damage pattern for DCP1, DCP2, DCP3, and DCP4 by running the required file from both the carriers

cd ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/
for case in */
do
  cd $case/
  for cost in */
  do
    cd $cost/
    for phase in */
    do
      if [ "$phase" = "phase_02/" ]
      then
         cd $phase/
         # instance_files_for_carrier_a='ls ./carrier_0/*.conf'
         # instance_files_for_carrier_b='ls ./carrier_1/*.conf'
         cd carrier_0/
         
         
         #echo "I am at $case $cost $phase carrier_0/"  #/rcon_milp41_esen_dci_cspt_solution_ana_v3/build/  -->/cspt/
         for g in *.conf
              do
                cp -f carrier_0.input.request ~/Ignazio_ECOC_2025/cspt/${g%%.*}.input.request #original_request_modification
                cp -f ${g%%.*}.input.conf ~/Ignazio_ECOC_2025/cspt/${g%%.*}.input.conf
                cp -f ${g%%.*}.input.price ~/Ignazio_ECOC_2025/cspt/${g%%.*}.input.price
                cp -f ${g%%.*}.input.links ~/Ignazio_ECOC_2025/Price_generation/Carrier_0_links.csv
                
                
                temp="${g%%.*}"
                temp1="$(cut -d'_' -f3 <<<"$temp")"
                #echo "$temp1"
                cd ..

      


                cd carrier_1/
                
                temp_dir="$HOME/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/$case$cost$phase"
                temp_dir_p3="$HOME/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/$case$cost" 
                temp_dir_numer="$HOME/Ignazio_ECOC_2025/Numerical_Analysis/"
                temp_dir_base="$Home/Ignazio_ECOC_2025/"
                 
                for h in *.conf
                do
                  temp2="${h%%.*}"
                  temp3="$(cut -d'_' -f3 <<<"$temp2")"
                  if [ "$temp1" = "$temp3" ]
                  then
                   
                    cp -f carrier_1.input.request ~/Ignazio_ECOC_2025/cspt/${h%%.*}.input.request  #original_request_modification
                    cp -f ${h%%.*}.input.conf ~/Ignazio_ECOC_2025/cspt/${h%%.*}.input.conf
                    cp -f ${h%%.*}.input.price ~/Ignazio_ECOC_2025/cspt/${h%%.*}.input.price
                    cp -f ${h%%.*}.input.links ~/Ignazio_ECOC_2025/Price_generation/Carrier_1_links.csv
                    
                    cd ~/Ignazio_ECOC_2025/cspt/
                    
                    #Initial damage evaluation by three carriers
                   
                    sh ./damage_eva.sh ${g%%.*} 0 -e
                    sh ./damage_eva.sh ${h%%.*} 1 -e
                    #sh ./damage_eva.sh ${m%%.*} 2 -e
                    
                    
                    cp -f ${g%%.*}.output.links ~/Ignazio_ECOC_2025/DCP1_Heuristic/DCI_carrier_0_links.csv
                    cp -f ${h%%.*}.output.links ~/Ignazio_ECOC_2025/DCP1_Heuristic/DCI_carrier_1_links.csv
                    #cp -f ${m%%.*}.output.links ~/Ignazio_ECOC_2025/DCP1_Heuristic/DCI_carrier_2_links.csv
                    
                    cp -f ${g%%.*}.output.links ~/Ignazio_ECOC_2025/DCP2_Heuristic/DCI_carrier_0_links.csv
                    cp -f ${h%%.*}.output.links ~/Ignazio_ECOC_2025/DCP2_Heuristic/DCI_carrier_1_links.csv
                    #cp -f ${m%%.*}.output.links ~/Ignazio_ECOC_2025/DCP2_Heuristic/DCI_carrier_2_links.csv

                    

                    
                    cp -f ${g%%.*}.output.links $temp_dir_p3/phase_03/dcp_1/${g%%.*}.output.links
                    cp -f ${h%%.*}.output.links $temp_dir_p3/phase_03/dcp_1/${h%%.*}.output.links
                    cp -f ${g%%.*}.output.links $temp_dir_p3/phase_03/dcp_2/${g%%.*}.output.links
                    cp -f ${h%%.*}.output.links $temp_dir_p3/phase_03/dcp_2/${h%%.*}.output.links
                    
                    

                    cp -f ${g%%.*}.input.request ${g%%.*}.org.input.request
                    cp -f ${h%%.*}.input.request ${h%%.*}.org.input.request
                    #cp -f ${m%%.*}.input.request ${m%%.*}.org.input.request

                    cp -f ${g%%.*}.input.conf ${g%%.*}.org.input.conf
                    cp -f ${h%%.*}.input.conf ${h%%.*}.org.input.conf
                    #cp -f ${m%%.*}.input.conf ${m%%.*}.org.input.conf

                    cp -f ${g%%.*}.input.price ${g%%.*}.org.input.price
                    cp -f ${h%%.*}.input.price ${h%%.*}.org.input.price
                    #cp -f ${m%%.*}.input.price ${m%%.*}.org.input.price



                    
                    
                    #Numerical Analysis Addition
                    cd "$temp_dir_numer"
                    for instance in */
                    do
                      temp5="$(cut -d'_' -f2 <<<"$instance")"
  
                      temp6="$(cut -d'/' -f1 <<<"$temp5")"
                      #echo "I am in Numer"
                      #echo "$temp6"
                      if [ "$temp6" = "$temp3" ]
                      then
  
                         cd ~/Ignazio_ECOC_2025/cspt/
                         cp -f ${g%%.*}.* $temp_dir_numer$instance/Original_request/
                         cp -f ${h%%.*}.* $temp_dir_numer$instance/Original_request/
                         #cp -f ${m%%.*}.* $temp_dir_numer$instance/Original_request/
                         #cd "$temp_dir_numer"
                         break
                      fi
                    done
                    
                    #Numerical Analysis Addition Ends Here
                    
                    cd "$temp_dir"
                    
                    #Copying carrier links info after disaster to generate price_lists
                    cd ~/Ignazio_ECOC_2025/Price_generation/
                    
                    python3 Price_generation_New.py
                    
                    
                    #Call the Phase_3 Script
                    #cd ..
                    
                    #Phase 3
                    #copy integrated Price_lists to all the folder in Phase 3
                    



                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.txt $temp_dir_p3/phase_03/carrier_0/${g%%.*}.input.price
                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.txt $temp_dir_p3/phase_03/carrier_1/${h%%.*}.input.price
                    #cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.txt $temp_dir_p3/phase_03/carrier_2/${m%%.*}.input.price


                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.txt $temp_dir_p3/phase_03/dcp_1/Price_lists.$temp3.price
                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.txt $temp_dir_p3/phase_03/dcp_2/Price_lists.$temp3.price
                    #cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.txt $temp_dir_p3/phase_03/dcp_3/Price_lists.$temp3.price
                    #cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.txt $temp_dir_p3/phase_03/dcp_4/Price_lists.$temp3.price


                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.txt $temp_dir_p3/phase_03/pne/Price_lists.$temp3.price

                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.csv ~/Ignazio_ECOC_2025/DCP1_Heuristic/Price_lists.csv
                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.csv ~/Ignazio_ECOC_2025/DCP2_Heuristic/Price_lists.csv
                    #cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.csv ~/Ignazio_ECOC_2025/DCP3_Heuristic/Price_lists.csv
                    #cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.csv ~/Ignazio_ECOC_2025/DCP4_Heuristic/Price_lists.csv

                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.csv ~/Ignazio_ECOC_2025/DCP1_Set_Creation/Price_lists.csv
                    cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.csv ~/Ignazio_ECOC_2025/DCP2_Set_Creation/Price_lists.csv
                    #cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.csv ~/Ignazio_ECOC_2025/DCP3_Set_Creation/Price_lists.csv
                    #cp -f ~/Ignazio_ECOC_2025/Price_generation/Price_lists.csv ~/Ignazio_ECOC_2025/DCP4_Set_Creation/Price_lists.csv


                    
                    cd ..

                    #Content requests generation for all four DCPs

                    cd ~/Ignazio_ECOC_2025/DCP1_Content_Request_generation/

                    python3 Request_generation.py
                    cp -f Final_content_requests.csv ~/Ignazio_ECOC_2025/DCP1_Heuristic/Final_content_requests.csv

                    cd ..

                    cd ~/Ignazio_ECOC_2025/DCP2_Content_Request_generation/

                    python3 Request_generation.py
                    cp -f Final_content_requests.csv ~/Ignazio_ECOC_2025/DCP2_Heuristic/Final_content_requests.csv

                    cd ..

                    

                    #############################################
                    #...............DCPs section................#
                    #############################################



                    #For DCP1..................#
                    cd ~/Ignazio_ECOC_2025/DCP1_Heuristic/
                    
                    #Run DCI_Link_AD_generation for generating survived links of DCI
                    echo "Working with DCP1_Heuristic............"
                    python3 DCI_Link_AD_generation.py
                    python3 Heuristic_DCP1_User_Redirect.py

                    cp -f DCP1_Set1.csv ~/Ignazio_ECOC_2025/DCP1_Set_Creation/DCP1_Set1.csv
                    cp -f DCP1_Requests_1.csv ~/Ignazio_ECOC_2025/DCP1_Set_Creation/DCP1_Requests_1.csv
                    cp -f links_DCP1_AD.csv ~/Ignazio_ECOC_2025/DCP1_Set_Creation/links_DCP1_AD.csv
                    
                    echo "Done with DCP1_Heuristic............"
                    cd ..
                    #Run Set Creator
                    cd ~/Ignazio_ECOC_2025/DCP1_Set_Creation/
                    
                    echo "Creating alternative set for DCP1............"
                    python3 Set_Creation_DCP1.py
                    
                    echo "Done with creation of alternative set for DCP1............"
                    cd ..

                    
                    
                    #For DCP2..................#
                    cd ~/Ignazio_ECOC_2025/DCP2_Heuristic/
                    
                    #Run DCI_Link_AD_generation for generating survived links of DCI
                    echo "Working with DCP2_Heuristic............"
                    python3 DCI_Link_AD_generation.py
                    python3 Heuristic_DCP2_User_Redirect.py

                    cp -f DCP2_Set1.csv ~/Ignazio_ECOC_2025/DCP2_Set_Creation/DCP2_Set1.csv
                    cp -f DCP2_Requests_1.csv ~/Ignazio_ECOC_2025/DCP2_Set_Creation/DCP2_Requests_1.csv
                    cp -f links_DCP2_AD.csv ~/Ignazio_ECOC_2025/DCP2_Set_Creation/links_DCP2_AD.csv
                    
                    echo "Done with DCP2_Heuristic............"
                    cd ..
                    #Run Set Creator
                    cd ~/Ignazio_ECOC_2025/DCP2_Set_Creation/
                    
                    echo "Creating alternative set for DCP2............"
                    python3 Set_Creation_DCP2.py
                    
                    echo "Done with creation of alternative set for DCP2............"
                    cd ..


                    
                    
                    
                    #End of Phase_3
                    
                    #Phase 4
                    
                    #Numerical Analysis Addition
                    cd "$temp_dir_numer"
                    for instance in */
                    do
                      temp5="$(cut -d'_' -f2 <<<"$instance")"
  
                      temp6="$(cut -d'/' -f1 <<<"$temp5")"
                      #echo "I am in Numer"
                      #echo "$temp6"
                      if [ "$temp6" = "$temp3" ]
                      then
  
                         #cd ~/ESEN-DCI/Heuristics_Final/
                         cp -f ~/Ignazio_ECOC_2025/DCP1_Set_Creation/DCP1_Requests_1.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_1.csv
                         cp -f ~/Ignazio_ECOC_2025/DCP1_Set_Creation/DCP1_Requests_2.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_2.csv
                         cp -f ~/Ignazio_ECOC_2025/DCP1_Heuristic/Service_Restoration_Content_User_requests.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/Service_Restoration_Content_User_requests.csv
                         #cp -f ~/Ignazio_ECOC_2025/DCP1_Set_Creation/DCP1_Requests_1.csv ~/Ignazio_ECOC_2025/PNE/Set_Combination/DCP1_Requests_1.csv
                         #cp -f ~/Ignazio_ECOC_2025/DCP1_Set_Creation/DCP1_Requests_2.csv ~/Ignazio_ECOC_2025/PNE/Set_Combination/DCP1_Requests_2.csv
                         
                         cp -f ~/Ignazio_ECOC_2025/DCP2_Set_Creation/DCP2_Requests_1.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_1.csv
                         cp -f ~/Ignazio_ECOC_2025/DCP2_Set_Creation/DCP2_Requests_2.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_2.csv
                         cp -f ~/Ignazio_ECOC_2025/DCP2_Heuristic/Service_Restoration_Content_User_requests.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/Service_Restoration_Content_User_requests.csv
                         #cp -f ~/Ignazio_ECOC_2025/DCP2_Set_Creation/DCP2_Requests_1.csv ~/Ignazio_ECOC_2025/PNE/Set_Combination/DCP2_Requests_1.csv
                         #cp -f ~/Ignazio_ECOC_2025/DCP2_Set_Creation/DCP2_Requests_2.csv ~/Ignazio_ECOC_2025/PNE/Set_Combination/DCP2_Requests_2.csv
                         
                         



                         break
                      fi
                    done
                    
                    #Numerical Analysis Addition Ends Here
                    
                    
                    
                    
                    
                            
                    cp -f ~/Ignazio_ECOC_2025/DCP1_Heuristic/Service_Restoration_Content_User_requests.csv $temp_dir_p3/phase_05/dcp_1/Service_Restoration_Content_User_requests.csv
                    cp -f ~/Ignazio_ECOC_2025/DCP2_Heuristic/Service_Restoration_Content_User_requests.csv $temp_dir_p3/phase_05/dcp_2/Service_Restoration_Content_User_requests.csv
                    #cp -f ~/Ignazio_ECOC_2025/DCP3_Heuristic/Service_Restoration_Content_User_requests.csv $temp_dir_p3/phase_05/dcp_3/Service_Restoration_Content_User_requests.csv
                    #cp -f ~/Ignazio_ECOC_2025/DCP4_Heuristic/Service_Restoration_Content_User_requests.csv $temp_dir_p3/phase_05/dcp_4/Service_Restoration_Content_User_requests.csv
                    
                    
                    
                    
                    
                    
                    
                    cd "$temp_dir"
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    #End of Phase_5
                    
                    
                    
                    
                    cd "$temp_dir"
                    cd carrier_1/
                  fi
                done
                cd ..
                cd carrier_0/
                
                
                
                
              done   
         
         cd ..
         
         
         
    
         
         
         
         cd ..
      fi    
    done
    cd ..
  done
  cd ..
done

cd "$HOME/Ignazio_ECOC_2025/"
#chmod +x 2025_Globecom_Extension_data_Script_final.sh
#./2025_Globecom_Extension_data_Script_final.sh            
             
                
                
                
                
                
                
                
                
                
                
