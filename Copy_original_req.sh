cd
HOME=`pwd`


cp -f ~/Ignazio_ECOC_2025/Requests/carrier_0.input.request ~/Ignazio_ECOC_2025/cspt/carrier_0.input.request
cp -f ~/Ignazio_ECOC_2025/Requests/carrier_1.input.request ~/Ignazio_ECOC_2025/cspt/carrier_1.input.request
#cp -f ~/Ignazio_ECOC_2025/Requests/carrier_2.input.request ~/Ignazio_ECOC_2025/cspt/carrier_2.input.request
#Phase 2
#generating damage pattern for DCP1 running the required file from both the carriers
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
         cp -f ~/Ignazio_ECOC_2025/Requests/carrier_0.input.request carrier_0.input.request
         cd ..
         cd carrier_1/
         cp -f ~/Ignazio_ECOC_2025/Requests/carrier_1.input.request carrier_1.input.request
         cd ..
   #      cd carrier_2/
   #      cp -f ~/Ignazio_ECOC_2025/Requests/carrier_2.input.request carrier_2.input.request
   #      cd ..
         
    
         
         
         
         cd ..
      fi    
    done
    cd ..
  done
  cd ..
done

                
                
                
                
                
                
                
                
                
                
