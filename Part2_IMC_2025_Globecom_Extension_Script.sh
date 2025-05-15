cd
HOME=`pwd`

#This script is to do the followings:
#1) Copy the DCP 1 and DCP2 requests (Set 1 and Set 2) to the PNE folder
#2) Run the global optimization by PNE
#3) Get the outputs and copy them to the Result analysis and Numerical_Instances folders
#4) Run the result analysis for each instances
#5) Copy the results to the Numerical_Instances folders

temp_dir="$HOME/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/$case$cost$phase"
temp_dir_p3="$HOME/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/$case$cost" 
temp_dir_numer="$HOME/Ignazio_ECOC_2025/Numerical_Analysis/"
temp_dir_base="$HOME/Ignazio_ECOC_2025/"


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

      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP1_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP1_Requests_2.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP2_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP2_Requests_2.csv

      #need to copy config files of Carrier 0 and 1 to ILP and KSP folders

      cd ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/
      for case in */
      do
        cd ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/

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
         
         
                for g in *.conf
                do
                  temp="${g%%.*}"
                  temp1="$(cut -d'_' -f3 <<<"$temp")"
                  #echo "$temp1"
                  if [ "$temp1" = "$temp6" ]
                  then 
#                    echo "We are copying the links files     ###############"
#                    pwd
		    cp -f ${g%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_ILP/Carrier_0_links.csv
                    cp -f ${g%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_KSP/Carrier_0_links.csv
                  fi

                  cd ..

                  pwd
                  cd carrier_1/

                  for h in *.conf
                  do
                    temp2="${h%%.*}"
                    temp3="$(cut -d'_' -f3 <<<"$temp2")"
                    if [ "$temp1" = "$temp3" ]
                    then
                   
                      cp -f ${h%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_ILP/Carrier_1_links.csv
                      cp -f ${h%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_KSP/Carrier_1_links.csv
                    fi
                  done
                  cd ..
                  cd carrier_0/
                done
            fi
          done
        done
      done








      #done with 
      

      #Run ILP and KSP
      cd ~/Ignazio_ECOC_2025/Ignazio_ILP/
      echo "Running ILP for Heavy Damage............"
      
      python3 Ignazio_ILP1.py

      echo "Done with Running ILP for Heavy Damage............"

      cp -f ilp1_carrier0_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/ilp1_carrier0_Final.csv
      cp -f ilp1_carrier1_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/ilp1_carrier1_Final.csv

      cp -f ilp1_carrier0_Final.csv $temp_dir_numer$instance/FinalResults/ilp1_carrier0_Final.csv
      cp -f ilp1_carrier1_Final.csv $temp_dir_numer$instance/FinalResults/ilp1_carrier1_Final.csv


      #output ilp1_carrier0_Final.csv
      #output ilp1_carrier1_Final.csv



      cd ~/Ignazio_ECOC_2025/Ignazio_KSP/
      echo "Running KSP for Heavy Damage............"
      python3 IMC_KSP.py

      echo "Done with Running KSP for Heavy Damage............"

      cp -f ksp_carrier0_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/ksp_carrier0_Final.csv
      cp -f ksp_carrier1_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/ksp_carrier1_Final.csv

      cp -f ksp_carrier0_Final.csv $temp_dir_numer$instance/FinalResults/ksp_carrier0_Final.csv
      cp -f ksp_carrier1_Final.csv $temp_dir_numer$instance/FinalResults/ksp_carrier1_Final.csv


      cd ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/

      python3 Result_analysis_dcp1.py
      python3 Result_analysis_dcp2.py
      cp -f DCP1_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv
      cp -f DCP2_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv
      cp -f Combined_final_result.csv $temp_dir_numer$instance/DCI_Analysis/Combined/$temp6.ILP.Combined_final_result.csv

      #copy the output files to numerical analysis

      cd ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/

      python3 Result_analysis_dcp1.py
      python3 Result_analysis_dcp2.py
      cp -f DCP1_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv
      cp -f DCP2_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv
      cp -f Combined_final_result.csv $temp_dir_numer$instance/DCI_Analysis/Combined/$temp6.KSP.Combined_final_result.csv

      #copy the output files to numerical analysis

      #break
#      cd "$temp_dir_numer"
      cd "$temp_dir_base"

  fi



  if [ "$temp6" -ge 200 ] && [ "$temp6" -le 249 ];
    then
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP1_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP1_Requests_2.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP2_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP2_Requests_2.csv

      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP1_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP1_Requests_2.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP2_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP2_Requests_2.csv

      #need to copy config files of Carrier 0 and 1 to ILP and KSP folders

      cd ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/
      for case in */
      do
        cd ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/

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
         
         
                for g in *.conf
                do
#                  pwd
		  #cd carrier_0
 
		  temp="${g%%.*}"
                  temp1="$(cut -d'_' -f3 <<<"$temp")"
                  #echo "$temp1"
                  if [ "$temp1" = "$temp6" ]
                  then 
#                    echo "We are copying the links files for carrier 0     ###############"
#                    pwd
		    cp -f ${g%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_ILP/Carrier_0_links.csv
                    cp -f ${g%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_KSP/Carrier_0_links.csv
                  fi

                  cd ..

                  cd carrier_1/

                  for h in *.conf
                  do
                    temp2="${h%%.*}"
                    temp3="$(cut -d'_' -f3 <<<"$temp2")"
                    if [ "$temp1" = "$temp3" ]
                    then
#                      echo "We are copying the links files for carrier 1     ###############"
#                      pwd
	                  
                      cp -f ${h%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_ILP/Carrier_1_links.csv
                      cp -f ${h%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_KSP/Carrier_1_links.csv
                    fi
                  done
                  cd ..
                  cd carrier_0/
              
                done
            fi
          done
        done
      done








      #done with 
      

      #Run ILP and KSP
      cd ~/Ignazio_ECOC_2025/Ignazio_ILP/
      echo "Running ILP for Heavy Damage............"
      
      python3 Ignazio_ILP1.py

      echo "Done with Running ILP for Heavy Damage............"

      cp -f ilp1_carrier0_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/ilp1_carrier0_Final.csv
      cp -f ilp1_carrier1_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/ilp1_carrier1_Final.csv

      cp -f ilp1_carrier0_Final.csv $temp_dir_numer$instance/FinalResults/ilp1_carrier0_Final.csv
      cp -f ilp1_carrier1_Final.csv $temp_dir_numer$instance/FinalResults/ilp1_carrier1_Final.csv


      #output ilp1_carrier0_Final.csv
      #output ilp1_carrier1_Final.csv



      cd ~/Ignazio_ECOC_2025/Ignazio_KSP/
      echo "Running KSP for Heavy Damage............"
      python3 IMC_KSP.py

      echo "Done with Running KSP for Heavy Damage............"

      cp -f ksp_carrier0_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/ksp_carrier0_Final.csv
      cp -f ksp_carrier1_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/ksp_carrier1_Final.csv

      cp -f ksp_carrier0_Final.csv $temp_dir_numer$instance/FinalResults/ksp_carrier0_Final.csv
      cp -f ksp_carrier1_Final.csv $temp_dir_numer$instance/FinalResults/ksp_carrier1_Final.csv


      cd ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/

      python3 Result_analysis_dcp1.py
      python3 Result_analysis_dcp2.py
      cp -f DCP1_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv
      cp -f DCP2_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv
      cp -f Combined_final_result.csv $temp_dir_numer$instance/DCI_Analysis/Combined/$temp6.ILP.Combined_final_result.csv

      #copy the output files to numerical analysis

      cd ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/

      python3 Result_analysis_dcp1.py
      python3 Result_analysis_dcp2.py
      cp -f DCP1_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv
      cp -f DCP2_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv
      cp -f Combined_final_result.csv $temp_dir_numer$instance/DCI_Analysis/Combined/$temp6.KSP.Combined_final_result.csv

      #copy the output files to numerical analysis

      #break
      cd "$temp_dir_base"
  fi

  if [ "$temp6" -ge 350 ] && [ "$temp6" -le 399 ];
    then
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP1_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP1_Requests_2.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP2_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_ILP/DCP2_Requests_2.csv

      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP1_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP1/DCP1_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP1_Requests_2.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_1.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP2_Requests_1.csv
      cp -f $temp_dir_numer$instance/DCI_Analysis/DCP2/DCP2_Requests_2.csv ~/Ignazio_ECOC_2025/Ignazio_KSP/DCP2_Requests_2.csv

      #need to copy config files of Carrier 0 and 1 to ILP and KSP folders

      cd ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/
      for case in */
      do
        cd ~/Ignazio_ECOC_2025/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1/build/scenario/after_disaster/
 
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
         
         
                for g in *.conf
                do
 
		  temp="${g%%.*}"
                  temp1="$(cut -d'_' -f3 <<<"$temp")"
                  #echo "$temp1"
                  if [ "$temp1" = "$temp6" ]
                  then 
#                    echo "We are copying the links files carrier 0    ###############"
#                    pwd

		    cp -f ${g%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_ILP/Carrier_0_links.csv
                    cp -f ${g%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_KSP/Carrier_0_links.csv
                  fi

                  cd ..

                  cd carrier_1/

                  for h in *.conf
                  do
                    temp2="${h%%.*}"
                    temp3="$(cut -d'_' -f3 <<<"$temp2")"
                    if [ "$temp1" = "$temp3" ]
                    then
#                      echo "We are copying the links files carrier 1    ###############"
#                      pwd
                  
                      cp -f ${h%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_ILP/Carrier_1_links.csv
                      cp -f ${h%%.*}.input.links ~/Ignazio_ECOC_2025/Ignazio_KSP/Carrier_1_links.csv
                    fi
                  done
                  cd ..
                  cd carrier_0/
	    done
            fi
          done
        done
      done








      #done with 
      

      #Run ILP and KSP
      cd ~/Ignazio_ECOC_2025/Ignazio_ILP/
      echo "Running ILP for Heavy Damage............"
      
      python3 Ignazio_ILP1.py

      echo "Done with Running ILP for Heavy Damage............"

      cp -f ilp1_carrier0_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/ilp1_carrier0_Final.csv
      cp -f ilp1_carrier1_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/ilp1_carrier1_Final.csv

      cp -f ilp1_carrier0_Final.csv $temp_dir_numer$instance/FinalResults/ilp1_carrier0_Final.csv
      cp -f ilp1_carrier1_Final.csv $temp_dir_numer$instance/FinalResults/ilp1_carrier1_Final.csv


      #output ilp1_carrier0_Final.csv
      #output ilp1_carrier1_Final.csv



      cd ~/Ignazio_ECOC_2025/Ignazio_KSP/
      echo "Running KSP for Heavy Damage............"
      python3 IMC_KSP.py

      echo "Done with Running KSP for Heavy Damage............"

      cp -f ksp_carrier0_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/ksp_carrier0_Final.csv
      cp -f ksp_carrier1_Final.csv ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/ksp_carrier1_Final.csv

      cp -f ksp_carrier0_Final.csv $temp_dir_numer$instance/FinalResults/ksp_carrier0_Final.csv
      cp -f ksp_carrier1_Final.csv $temp_dir_numer$instance/FinalResults/ksp_carrier1_Final.csv


      cd ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/ILP/

      python3 Result_analysis_dcp1.py
      python3 Result_analysis_dcp2.py
      cp -f DCP1_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/ILP/$temp6.ILP.DCP1.Analyzed_Results.csv
      cp -f DCP2_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/ILP/$temp6.ILP.DCP2.Analyzed_Results.csv
      cp -f Combined_final_result.csv $temp_dir_numer$instance/DCI_Analysis/Combined/$temp6.ILP.Combined_final_result.csv

      #copy the output files to numerical analysis

      cd ~/Ignazio_ECOC_2025/Result_Analysis/Analysis_Each_Result/KSP/

      python3 Result_analysis_dcp1.py
      python3 Result_analysis_dcp2.py
      cp -f DCP1_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP1/KSP/$temp6.KSP.DCP1.Analyzed_Results.csv
      cp -f DCP2_Analyzed_Results.csv $temp_dir_numer$instance/DCI_Analysis/DCP2/KSP/$temp6.KSP.DCP2.Analyzed_Results.csv
      cp -f Combined_final_result.csv $temp_dir_numer$instance/DCI_Analysis/Combined/$temp6.KSP.Combined_final_result.csv

      #copy the output files to numerical analysis

      #break
#      cd "$temp_dir_numer"
      cd "$temp_dir_base"

  fi

  cd "$temp_dir_numer"

  
done




cd "~/Ignazio_ECOC_2025/"
#chmod +x 2025_Globecom_Extension_data_Script_final.sh
#./2025_Globecom_Extension_data_Script_final.sh            
             
                
                
                
                
                
                
                
                
                
                
