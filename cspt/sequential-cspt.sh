Recovery_Task_Per_Unit_Time=1

rm conf.txt
rm lightpath_price.txt
rm customer_lightpath_requests.txt
rm carrier_lightpath_requests.txt
rm connection_requests.txt

cp carrier_$2.original_requests original_requests.txt

cp $1.input.conf conf.txt
cp $1.input.price lightpath_price.txt
cp 0_requests carrier_lightpath_requests.txt
#cp $1.input.carrier_lightpath_request carrier_lightpath_requests.txt


if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp $1.input.request connection_requests.txt

   cat connection_requests.txt |grep carrier_id=2 > dcp0_connection_requests.tmp
   cat connection_requests.txt |grep carrier_id=3 > dcp1_connection_requests.tmp
   dcp0_path_amount=`wc -l < dcp0_connection_requests.tmp`
   dcp1_path_amount=`wc -l < dcp1_connection_requests.tmp`
   echo "Path#="$dcp0_path_amount "Customer_Carrier#=4" > dcp0_connection_requests.txt
   cat dcp0_connection_requests.tmp >> dcp0_connection_requests.txt
   echo "Path#="$dcp1_path_amount "Customer_Carrier#=4" > dcp1_connection_requests.txt
   cat dcp1_connection_requests.tmp >> dcp1_connection_requests.txt
   rm *.tmp

 #  cp dcp0_connection_requests.txt connection_requests.txt
 #  cp dcp0_connection_requests.txt $1.seq_1.connection_requests.txt

   cp 0_requests connection_requests.txt
   cp 0_requests $1.seq_1.connection_requests.txt

   cp 0_requests customer_lightpath_requests.txt
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp $1.input.request customer_lightpath_requests.txt

   cat customer_lightpath_requests.txt |grep carrier_id=2 > dcp0_lightpath_requests.tmp
   cat customer_lightpath_requests.txt |grep carrier_id=3 > dcp1_lightpath_requests.tmp

   dcp0_path_amount=`wc -l < dcp0_lightpath_requests.tmp`
   dcp1_path_amount=`wc -l < dcp1_lightpath_requests.tmp`
   echo "Path#="$dcp0_path_amount "Customer_Carrier#=4" > dcp0_lightpath_requests.txt
   cat dcp0_lightpath_requests.tmp >> dcp0_lightpath_requests.txt
   echo "Path#="$dcp1_path_amount "Customer_Carrier#=4" > dcp1_lightpath_requests.txt
   cat dcp1_lightpath_requests.tmp >> dcp1_lightpath_requests.txt
   rm *.tmp

#   cp dcp0_lightpath_requests.txt customer_lightpath_requests.txt
#   cp dcp0_lightpath_requests.txt $1.seq_1.lightpath_requests.txt
   cp 0_requests customer_lightpath_requests.txt
   cp 0_requests $1.seq_1.customer_lightpath_requests.txt

   cp 0_requests connection_requests.txt
fi



rm ee.mps
echo "###########################"
echo $1 Seq_1: Solving carrier"'"s request
echo "###########################"

./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_low_bound_v2 -m $2 > model.out

#old version ./rcon_milp41_esen_dci_cspt_v3  -m $2 > model.out
cat model.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model.1
cp  model.1 $1.seq_1.mod

rm ee.sol
~/cplex -c "read ee.mps" "set timelimit 7200" "set mip tol mipgap 0.0000000000001" "mipopt" "write ee.sol 1" > $1.seq_1.cplex.log
less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp ee.mps $1.seq_1.seq_1.mps

cp ee.sol $1.seq_1.seq_1.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat ee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.seq_1.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model.1 ee.1 > $1.seq_1.sol


cat $1.seq_1.sol |grep Bm > Bmn.tmp
wc -l < Bmn.tmp > Bmn_solution.temp
cat Bmn.tmp >> Bmn_solution.temp
cp Bmn_solution.temp $1.seq_1.Bmn_solution.temp


cat $1.seq_1.sol |grep Ca > Csd.tmp
wc -l < Csd.tmp > Csd_solution.temp
cat Csd.tmp >> Csd_solution.temp
cp Csd_solution.temp $1.seq_1.Csd_solution.temp

cat $1.seq_1.sol |grep Ps > Pmnijw.tmp
wc -l < Pmnijw.tmp > Pmnijw_solution.temp
cat Pmnijw.tmp >> Pmnijw_solution.temp
cp Pmnijw_solution.temp $1.seq_1.Pmnijw_solution.temp

cat $1.seq_1.sol |grep Mi > Mijmn.tmp
wc -l < Mijmn.tmp > Mijmn_solution.temp
cat Mijmn.tmp >> Mijmn_solution.temp
cp Mijmn_solution.temp $1.seq_1.Mijmn_solution.temp

cat $1.seq_1.sol |grep Xa > Xijsd.tmp
wc -l < Xijsd.tmp > Xijsd_solution.temp
cat Xijsd.tmp >> Xijsd_solution.temp
cp Xijsd_solution.temp $1.seq_1.Xijsd_solution.temp

cat $1.seq_1.sol |grep Ub > Ub.tmp
wc -l < Ub.tmp > Ub_solution.temp
cat Ub.tmp >> Ub_solution.temp
cp Ub_solution.temp  $1.seq_1.Ub_solution.temp

cat $1.seq_1.sol |grep Vz > Vijw.tmp
wc -l < Vijw.tmp > Vijw_solution.temp
cat Vijw.tmp >> Vijw_solution.temp
cp Vijw_solution.temp $1.seq_1.Vijw_solution.temp

cat $1.seq_1.sol |grep Oi > Oija.tmp
wc -l < Oija.tmp > Oija_solution.temp
cat Oija.tmp >> Oija_solution.temp
cp Oija_solution.temp $1.seq_1.Oija_solution.temp

cat $1.seq_1.sol |grep Di > Dij.tmp
wc -l < Dij.tmp > Dij_solution.temp
cat Dij.tmp >> Dij_solution.temp
cp Dij_solution.temp $1.seq_1.Dij_solution.temp



if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_low_bound_v2 $3 $2 > dci_IPoverWDM_request_lower_bound_solution.temp
   cp 0_requests dci_lightpath_request_lower_bound_solution.temp
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_low_bound_v2 $3 $2 > dci_lightpath_request_lower_bound_solution.temp
   cp 0_requests dci_IPoverWDM_request_lower_bound_solution.temp

fi
#cp dci_IPoverWDM_request_lower_bound_solution.temp $1.dci_IPoverWDM_request_lower_bound_solution.before-leadtime.seq_1.temp
#cp dci_lightpath_request_lower_bound_solution.temp $1.dci_lightpath_request_lower_bound_solution.before-leadtime.seq_1.temp


cp dci_lead_time_checklist.temp dci_lead_time_checklist.seq_1.temp
cp dci_lead_time_checklist.temp $1.dci_lead_time_checklist.seq_1.temp


#cp links.csv $1.output.links_lower_bound

#cp carrier_evaluation.csv $1.output.evaluation_lower_bound

rm *.tmp
#########

rm eee.mps
echo "###########################"
echo $1 leadtime
echo "###########################"

./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -l $2 $Recovery_Task_Per_Unit_Time > model-leadtime.out
cp ee.mps eee.mps
cat model-leadtime.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-leadtime.1
cp  model-leadtime.1 $1.leadtime.seq_1.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.00000000001" "mipopt" "write eee.sol 1" > $1.leadtime.seq_1.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.leadtime.seq_1.mps

cp eee.sol $1.leadtime.seq_1.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.leadtime.seq_1.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-leadtime.1 eee.1 > $1.leadtime.seq_1.sol


cat $1.leadtime.seq_1.sol |grep Qj > Qjg.tmp
wc -l < Qjg.tmp > Qjg_lead_time_solution.temp
cat Qjg.tmp >> Qjg_lead_time_solution.temp
cp Qjg_lead_time_solution.temp Qjg_lead_time_solution.seq_1.temp
cp Qjg_lead_time_solution.temp $1.seq_1.Qjg_lead_time_solution.temp

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp dci_IPoverWDM_request_lower_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_IPoverWDM_request_lower_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_1.temp
   cp dci_lead_time_solution.temp $1.output.dci_IPoverWDM_request_solution.seq_1.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_1

fi


if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp dci_lightpath_request_lower_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_lightpath_request_lower_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_1.temp
   cp dci_lead_time_solution.temp $1.output.dci_lightpath_request_solution.seq_1.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_1

fi


#cp links.csv $1.output.seq_1.links

#cp carrier_evaluation.csv $1.output.seq_1.evaluation

#cp links.csv $1.dcp1.output.links

###cat links.csv |awk -F "," 'BEGIN{i=1}{if(NR==1){print $0} else if($11 == 2) {print i++","$2","$3","$4","$5","$6","$7","$8","$9",0,"$11}} END{}' > $1.carrier.output.links

##cp carrier_evaluation.csv $1.carrier.output.evaluation





#########

rm eee.mps
echo "###########################"
echo $1 Seq_2: solving dcp1 request based on carrier allocation result
echo "###########################"

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
#   cp $1.input.request connection_requests.txt
#   cp $1.input.request $1.seq_2.connection_requests.txt
   cp dcp0_connection_requests.txt connection_requests.txt
   cp dcp0_connection_requests.txt $1.seq_2.connection_requests.txt


   cp 0_requests customer_lightpath_requests.txt
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
#   cp $1.input.request customer_lightpath_requests.txt
#   cp $1.input.request $1.seq_2.customer_lightpath_requests.txt

   cp dcp0_lightpath_requests.txt customer_lightpath_requests.txt
   cp dcp0_lightpath_requests.txt $1.seq_2.customer_lightpath_requests.txt
 

   cp 0_requests connection_requests.txt
fi
###################

./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 -m $2 > model-up.out
cp ee.mps eee.mps
cat model-up.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-up.1
cp  model-up.1 $1.seq_2.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.0000000000001" "mipopt" "write eee.sol 1" > $1.seq_2.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.seq_2.mps

cp eee.sol $1.seq_2.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.seq_2.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-up.1 eee.1 > $1.seq_2.sol


cat $1.seq_2.sol |grep Bm > Bmn.tmp
wc -l < Bmn.tmp > Bmn_solution.temp
cat Bmn.tmp >> Bmn_solution.temp
cp Bmn_solution.temp Bmn_up_solution.temp
cp Bmn_solution.temp $1.seq_2.Bmn_solution.temp

cat $1.seq_2.sol |grep Ca > Csd.tmp
wc -l < Csd.tmp > Csd_solution.temp
cat Csd.tmp >> Csd_solution.temp
cp Csd_solution.temp Csd_up_solution.temp
cp Csd_solution.temp $1.seq_2.Csd_solution.temp


cat $1.seq_2.sol |grep Ps > Pmnijw.tmp
wc -l < Pmnijw.tmp > Pmnijw_solution.temp
cat Pmnijw.tmp >> Pmnijw_solution.temp
cp Pmnijw_solution.temp Pmnijw_up_solution.temp
cp Pmnijw_solution.temp $1.seq_2.Pmnijw_solution.temp


cat $1.seq_2.sol |grep Mi > Mijmn.tmp
wc -l < Mijmn.tmp > Mijmn_solution.temp
cat Mijmn.tmp >> Mijmn_solution.temp
cp Mijmn_solution.temp Mijmn_up_solution.temp
cp Mijmn_solution.temp $1.seq_2.Mijmn_solution.temp


cat $1.seq_2.sol |grep Xa > Xijsd.tmp
wc -l < Xijsd.tmp > Xijsd_solution.temp
cat Xijsd.tmp >> Xijsd_solution.temp
cp Xijsd_solution.temp Xijsd_up_solution.temp
cp Xijsd_solution.temp $1.seq_2.Xijsd_solution.temp

cat $1.seq_2.sol |grep Ub > Ub.tmp
wc -l < Ub.tmp > Ub_solution.temp
cat Ub.tmp >> Ub_solution.temp
cp Ub_solution.temp  Ub_up_solution.temp
cp Ub_solution.temp  $1.seq_2.Ub_solution.temp

cat $1.seq_2.sol |grep Vz > Vijw.tmp
wc -l < Vijw.tmp > Vijw_solution.temp
cat Vijw.tmp >> Vijw_solution.temp
cp Vijw_solution.temp Vijw_up_solution.temp
cp Vijw_solution.temp $1.seq_2.Vijw_solution.temp

cat $1.seq_2.sol |grep Oi > Oija.tmp
wc -l < Oija.tmp > Oija_solution.temp
cat Oija.tmp >> Oija_solution.temp
cp Oija_solution.temp Oija_up_solution.temp
cp Oija_solution.temp $1.seq_2.Oija_solution.temp


cat $1.seq_2.sol |grep Di > Dij.tmp
wc -l < Dij.tmp > Dij_solution.temp
cat Dij.tmp >> Dij_solution.temp
cp Dij_solution.temp Dij_up_solution.temp
cp Dij_solution.temp $1.seq_2.Dij_solution.temp



if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 $3 $2 > dci_IPoverWDM_request_upper_bound_solution.temp
#   cp 0_requests dci_lightpath_request_upper_bound_solution.temp
#   cp dci_IPoverWDM_request_upper_bound_solution.temp  $1.output.dci_request_solution
#   cp dci_IPoverWDM_request_upper_bound_solution.temp dci_customer_request_solution.temp


fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 $3 $2 > dci_lightpath_request_upper_bound_solution.temp
#   cp 0_requests dci_IPoverWDM_request_upper_bound_solution.temp
#   cp dci_lightpath_request_upper_bound_solution.temp  $1.output.dci_request_solution
#   cp dci_lightpath_request_upper_bound_solution.temp dci_customer_request_upper_bound_solution.temp
fi
#########

cp dci_lead_time_checklist.temp dci_lead_time_checklist.seq_2.temp
cp dci_lead_time_checklist.temp $1.dci_lead_time_checklist.seq_2.temp



rm eee.mps
echo "###########################"
echo $1 leadtime
echo "###########################"

./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -l $2 $Recovery_Task_Per_Unit_Time > model-leadtime.out
cp ee.mps eee.mps
cat model-leadtime.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-leadtime.1
cp  model-leadtime.1 $1.leadtime.seq_2.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.000000000001" "mipopt" "write eee.sol 1" > $1.leadtime.seq_2.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.leadtime.seq_2.mps

cp eee.sol $1.leadtime.seq_2.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.leadtime.seq_2.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-leadtime.1 eee.1 > $1.leadtime.seq_2.sol


cat $1.leadtime.seq_2.sol |grep Qj > Qjg.tmp
wc -l < Qjg.tmp > Qjg_lead_time_solution.temp
cat Qjg.tmp >> Qjg_lead_time_solution.temp
cp Qjg_lead_time_solution.temp Qjg_lead_time_solution.seq_2.temp
cp Qjg_lead_time_solution.temp $1.seq_2.Qjg_lead_time_solution.temp

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp dci_IPoverWDM_request_upper_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_IPoverWDM_request_lower_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_2.temp
   cp dci_lead_time_solution.temp $1.output.dci_IPoverWDM_request_solution.seq_2.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_2

fi


if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp dci_lightpath_request_upper_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_lightpath_request_lower_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_2.temp
   cp dci_lead_time_solution.temp $1.output.dci_lightpath_request_solution.seq_2.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_2

fi

#cp links.csv $1.output.seq_2.links
#cp carrier_evaluation.csv $1.output.seq_2.evaluation
#pickup only the dcp2 with id=3 and renumber
cat links.csv |awk -F "," 'BEGIN{i=1}{if(NR==1){print $0} else if($11 == 2) {print i++","$2","$3","$4","$5","$6","$7","$8","$9",0,"$11}} END{}' > $1.dcp1.output.links
#cp links.csv $1.dcp2.output.links

cp carrier_evaluation.csv $1.dcp1.output.evaluation



rm *.tmp




#########

rm eee.mps
echo "###########################"
echo $1 Seq_3: solving dcp2 request based on dcp1 allocation result
echo "###########################"

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp $1.input.request connection_requests.txt
   cp $1.input.request $1.seq_3.connection_requests.txt
 
   cp 0_requests customer_lightpath_requests.txt
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp $1.input.request customer_lightpath_requests.txt
   cp $1.input.request $1.seq_3.customer_lightpath_requests.txt
   cp 0_requests connection_requests.txt
fi
###################

./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 -m $2 > model-up.out
cp ee.mps eee.mps
cat model-up.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-up.1
cp  model-up.1 $1.seq_3.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.0000000000001" "mipopt" "write eee.sol 1" > $1.seq_3.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.seq_3.mps

cp eee.sol $1.seq_3.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.seq_3.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-up.1 eee.1 > $1.seq_3.sol


cat $1.seq_3.sol |grep Bm > Bmn.tmp
wc -l < Bmn.tmp > Bmn_up_solution.temp
cat Bmn.tmp >> Bmn_up_solution.temp
cp Bmn_up_solution.temp $1.seq_3.Bmn_solution.temp


cat $1.seq_3.sol |grep Ca > Csd.tmp
wc -l < Csd.tmp > Csd_up_solution.temp
cat Csd.tmp >> Csd_up_solution.temp
cp Csd_up_solution.temp $1.seq_3.Csd_solution.temp

cat $1.seq_3.sol |grep Ps > Pmnijw.tmp
wc -l < Pmnijw.tmp > Pmnijw_up_solution.temp
cat Pmnijw.tmp >> Pmnijw_up_solution.temp
cp Pmnijw_up_solution.temp $1.seq_3.Pmnijw_solution.temp

cat $1.seq_3.sol |grep Mi > Mijmn.tmp
wc -l < Mijmn.tmp > Mijmn_up_solution.temp
cat Mijmn.tmp >> Mijmn_up_solution.temp
cp Mijmn_up_solution.temp $1.seq_3.Mijmn_solution.temp

cat $1.seq_3.sol |grep Xa > Xijsd.tmp
wc -l < Xijsd.tmp > Xijsd_up_solution.temp
cat Xijsd.tmp >> Xijsd_up_solution.temp
cp Xijsd_up_solution.temp $1.seq_3.Xijsd_solution.temp

cat $1.seq_3.sol |grep Ub > Ub.tmp
wc -l < Ub.tmp > Ub_up_solution.temp
cat Ub.tmp >> Ub_up_solution.temp
cp Ub_up_solution.temp  $1.seq_3.Ub_solution.temp

cat $1.seq_3.sol |grep Vz > Vijw.tmp
wc -l < Vijw.tmp > Vijw_up_solution.temp
cat Vijw.tmp >> Vijw_up_solution.temp
cp Vijw_up_solution.temp $1.seq_3.Vijw_solution.temp

cat $1.seq_3.sol |grep Oi > Oija.tmp
wc -l < Oija.tmp > Oija_up_solution.temp
cat Oija.tmp >> Oija_up_solution.temp
cp Oija_up_solution.temp $1.seq_3.Oija_solution.temp

cat $1.seq_3.sol |grep Di > Dij.tmp
wc -l < Dij.tmp > Dij_up_solution.temp
cat Dij.tmp >> Dij_up_solution.temp
cp Dij_up_solution.temp $1.seq_3.Dij_solution.temp


if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 $3 $2 > dci_IPoverWDM_request_upper_bound_solution.temp
#   cp 0_requests dci_lightpath_request_upper_bound_solution.temp
#   cp dci_IPoverWDM_request_upper_bound_solution.temp  $1.output.dci_request_solution
#   cp dci_IPoverWDM_request_upper_bound_solution.temp dci_customer_request_solution.temp


fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 $3 $2 > dci_lightpath_request_upper_bound_solution.temp
#   cp 0_requests dci_IPoverWDM_request_upper_bound_solution.temp
#   cp dci_lightpath_request_upper_bound_solution.temp  $1.output.dci_request_solution
#   cp dci_lightpath_request_upper_bound_solution.temp dci_customer_request_upper_bound_solution.temp
fi
#########

cp dci_lead_time_checklist.temp dci_lead_time_checklist.seq_3.temp
cp dci_lead_time_checklist.temp $1.dci_lead_time_checklist.seq_3.temp



rm eee.mps
echo "###########################"
echo $1 leadtime
echo "###########################"

./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -l $2 $Recovery_Task_Per_Unit_Time > model-leadtime.out
cp ee.mps eee.mps
cat model-leadtime.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-leadtime.1
cp  model-leadtime.1 $1.leadtime.seq_3.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.00000000001" "mipopt" "write eee.sol 1" > $1.leadtime.seq_3.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.leadtime.seq_3.mps

cp eee.sol $1.leadtime.seq_3.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.leadtime.seq_3.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-leadtime.1 eee.1 > $1.leadtime.seq_3.sol


cat $1.leadtime.seq_3.sol |grep Qj > Qjg.tmp
wc -l < Qjg.tmp > Qjg_lead_time_solution.temp
cat Qjg.tmp >> Qjg_lead_time_solution.temp
cp Qjg_lead_time_solution.temp Qjg_lead_time_solution.seq_2.temp
cp Qjg_lead_time_solution.temp $1.seq_3.Qjg_lead_time_solution.temp

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp dci_IPoverWDM_request_upper_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_IPoverWDM_request_upper_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_3.temp
   cp dci_lead_time_solution.temp $1.output.dci_IPoverWDM_request_solution.seq_3.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_3

fi


if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp dci_lightpath_request_upper_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_lightpath_request_upper_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_3.temp
   cp dci_lead_time_solution.temp $1.output.dci_lightpath_request_solution.seq_3.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_3

fi

#cp links.csv $1.output.seq_2.links
#cp carrier_evaluation.csv $1.output.seq_2.evaluation
#pickup only the dcp2 with id=3 and renumber
cat links.csv |awk -F "," 'BEGIN{i=1}{if(NR==1){print $0} else if($11 == 3) {print i++","$2","$3","$4","$5","$6","$7","$8","$9",0,"$11}} END{}' > $1.dcp2.output.links
#cp links.csv $1.dcp2.output.links

cp carrier_evaluation.csv $1.dcp2.output.evaluation



rm *.tmp

exit

#########
# Done seq_1 and seq_2 
#########



if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."

   cp dcp1_connection_requests.txt connection_requests.txt
   cp dcp1_connection_requests.txt $1.seq_3.connection_requests.txt


   cp 0_requests customer_lightpath_requests.txt
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp dcp1_lightpath_requests.txt customer_lightpath_requests.txt
   cp dcp1_lightpath_requests.txt $1.seq_3.customer_lightpath_requests.txt


   cp 0_requests connection_requests.txt
fi



rm ee.mps
echo "###########################"
echo $1 seq_3 lower bound
echo "###########################"

./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_low_bound_v2 -m $2 > model.out

#old version ./rcon_milp41_esen_dci_cspt_v3  -m $2 > model.out
cat model.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model.1
cp  model.1 $1.seq_3.mod

rm ee.sol
~/cplex -c "read ee.mps" "set timelimit 7200" "set mip tol mipgap 0.0000000000001" "mipopt" "write ee.sol 1" > $1.seq_3.cplex.log
less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp ee.mps $1.seq_3.mps

cp ee.sol $1.seq_3.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat ee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.seq_3.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model.1 ee.1 > $1.seq_3.sol


cat $1.seq_3.sol |grep Bm > Bmn.tmp
wc -l < Bmn.tmp > Bmn_solution.temp
cat Bmn.tmp >> Bmn_solution.temp
cp Bmn_solution.temp $1.seq_3.Bmn_solution.temp


cat $1.seq_3.sol |grep Ca > Csd.tmp
wc -l < Csd.tmp > Csd_solution.temp
cat Csd.tmp >> Csd_solution.temp
cp Csd_solution.temp $1.seq_3.Csd_solution.temp

cat $1.seq_3.sol |grep Ps > Pmnijw.tmp
wc -l < Pmnijw.tmp > Pmnijw_solution.temp
cat Pmnijw.tmp >> Pmnijw_solution.temp
cp Pmnijw_solution.temp $1.seq_3.Pmnijw_solution.temp

cat $1.seq_3.sol |grep Mi > Mijmn.tmp
wc -l < Mijmn.tmp > Mijmn_solution.temp
cat Mijmn.tmp >> Mijmn_solution.temp
cp Mijmn_solution.temp $1.seq_3.Mijmn_solution.temp

cat $1.seq_3.sol |grep Xa > Xijsd.tmp
wc -l < Xijsd.tmp > Xijsd_solution.temp
cat Xijsd.tmp >> Xijsd_solution.temp
cp Xijsd_solution.temp $1.seq_3.Xijsd_solution.temp

cat $1.seq_3.sol |grep Ub > Ub.tmp
wc -l < Ub.tmp > Ub_solution.temp
cat Ub.tmp >> Ub_solution.temp
cp Ub_solution.temp  $1.seq_3.Ub_solution.temp

cat $1.seq_3.sol |grep Vz > Vijw.tmp
wc -l < Vijw.tmp > Vijw_solution.temp
cat Vijw.tmp >> Vijw_solution.temp
cp Vijw_solution.temp $1.seq_3.Vijw_solution.temp

cat $1.seq_3.sol |grep Oi > Oija.tmp
wc -l < Oija.tmp > Oija_solution.temp
cat Oija.tmp >> Oija_solution.temp
cp Oija_solution.temp $1.seq_3.Oija_solution.temp

cat $1.seq_3.sol |grep Di > Dij.tmp
wc -l < Dij.tmp > Dij_solution.temp
cat Dij.tmp >> Dij_solution.temp
cp Dij_solution.temp $1.seq_3.Dij_solution.temp



if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_low_bound_v2 $3 $2 > dci_IPoverWDM_request_lower_bound_solution.temp
   cp 0_requests dci_lightpath_request_lower_bound_solution.temp
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_low_bound_v2 $3 $2 > dci_lightpath_request_lower_bound_solution.temp
   cp 0_requests dci_IPoverWDM_request_lower_bound_solution.temp

fi
#cp dci_IPoverWDM_request_lower_bound_solution.temp $1.dci_IPoverWDM_request_lower_bound_solution.before-leadtime.seq_3.temp
#cp dci_lightpath_request_lower_bound_solution.temp $1.dci_lightpath_request_lower_bound_solution.before-leadtime.seq_3.temp


cp dci_lead_time_checklist.temp dci_lead_time_checklist.seq_3.temp
cp dci_lead_time_checklist.temp $1.dci_lead_time_checklist.seq_3.temp


#cp links.csv $1.output.links_lower_bound

#cp carrier_evaluation.csv $1.output.evaluation_lower_bound

rm *.tmp
#########

rm eee.mps
echo "###########################"
echo $1 leadtime
echo "###########################"

./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -l $2 $Recovery_Task_Per_Unit_Time > model-leadtime.out
cp ee.mps eee.mps
cat model-leadtime.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-leadtime.1
cp  model-leadtime.1 $1.leadtime.seq_3.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.00000000001" "mipopt" "write eee.sol 1" > $1.leadtime.seq_3.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.leadtime.seq_3.mps

cp eee.sol $1.leadtime.seq_3.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.leadtime.seq_3.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-leadtime.1 eee.1 > $1.leadtime.seq_3.sol


cat $1.leadtime.seq_3.sol |grep Qj > Qjg.tmp
wc -l < Qjg.tmp > Qjg_lead_time_solution.temp
cat Qjg.tmp >> Qjg_lead_time_solution.temp
cp Qjg_lead_time_solution.temp Qjg_lead_time_solution.seq_3.temp
cp Qjg_lead_time_solution.temp $1.seq_3.Qjg_lead_time_solution.temp

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp dci_IPoverWDM_request_lower_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_IPoverWDM_request_lower_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_3.temp
   cp dci_lead_time_solution.temp $1.output.dci_IPoverWDM_request_solution.seq_3.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_3

fi


if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp dci_lightpath_request_lower_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_lightpath_request_lower_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_3.temp
   cp dci_lead_time_solution.temp $1.output.dci_lightpath_request_solution.seq_3.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_3

fi


cp links.csv $1.output.seq_3.links

cp carrier_evaluation.csv $1.output.seq_3.evaluation






#########

rm eee.mps
echo "###########################"
echo $1 seq_4 lower bound
echo "###########################"

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp $1.input.request connection_requests.txt
   cp $1.input.request $1.seq_4.connection_requests.txt
 
   cp 0_requests customer_lightpath_requests.txt
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp $1.input.request customer_lightpath_requests.txt
   cp $1.input.request $1.seq_4.customer_lightpath_requests.txt
 
   cp 0_requests connection_requests.txt
fi
###################

./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 -m $2 > model-up.out
cp ee.mps eee.mps
cat model-up.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-up.1
cp  model-up.1 $1.seq_4.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.0000000000001" "mipopt" "write eee.sol 1" > $1.seq_4.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.seq_4.mps

cp eee.sol $1.seq_4.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.seq_4.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-up.1 eee.1 > $1.seq_4.sol


cat $1.seq_4.sol |grep Bm > Bmn.tmp
wc -l < Bmn.tmp > Bmn_up_solution.temp
cat Bmn.tmp >> Bmn_up_solution.temp
cp Bmn_up_solution.temp $1.seq_4.Bmn_solution.temp


cat $1.seq_4.sol |grep Ca > Csd.tmp
wc -l < Csd.tmp > Csd_up_solution.temp
cat Csd.tmp >> Csd_up_solution.temp
cp Csd_up_solution.temp $1.seq_4.Csd_solution.temp

cat $1.seq_4.sol |grep Ps > Pmnijw.tmp
wc -l < Pmnijw.tmp > Pmnijw_up_solution.temp
cat Pmnijw.tmp >> Pmnijw_up_solution.temp
cp Pmnijw_up_solution.temp $1.seq_4.Pmnijw_solution.temp

cat $1.seq_4.sol |grep Mi > Mijmn.tmp
wc -l < Mijmn.tmp > Mijmn_up_solution.temp
cat Mijmn.tmp >> Mijmn_up_solution.temp
cp Mijmn_up_solution.temp $1.seq_4.Mijmn_solution.temp

cat $1.seq_4.sol |grep Xa > Xijsd.tmp
wc -l < Xijsd.tmp > Xijsd_up_solution.temp
cat Xijsd.tmp >> Xijsd_up_solution.temp
cp Xijsd_up_solution.temp $1.seq_4.Xijsd_solution.temp

cat $1.seq_4.sol |grep Ub > Ub.tmp
wc -l < Ub.tmp > Ub_up_solution.temp
cat Ub.tmp >> Ub_up_solution.temp
cp Ub_up_solution.temp  $1.seq_4.Ub_solution.temp

cat $1.seq_4.sol |grep Vz > Vijw.tmp
wc -l < Vijw.tmp > Vijw_up_solution.temp
cat Vijw.tmp >> Vijw_up_solution.temp
cp Vijw_up_solution.temp $1.seq_4.Vijw_solution.temp

cat $1.seq_4.sol |grep Oi > Oija.tmp
wc -l < Oija.tmp > Oija_up_solution.temp
cat Oija.tmp >> Oija_up_solution.temp
cp Oija_up_solution.temp $1.seq_4.Oija_solution.temp

cat $1.seq_4.sol |grep Di > Dij.tmp
wc -l < Dij.tmp > Dij_up_solution.temp
cat Dij.tmp >> Dij_up_solution.temp
cp Dij_up_solution.temp $1.seq_4.Dij_solution.temp


if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 $3 $2 > dci_IPoverWDM_request_upper_bound_solution.temp
#   cp 0_requests dci_lightpath_request_upper_bound_solution.temp
#   cp dci_IPoverWDM_request_upper_bound_solution.temp  $1.output.dci_request_solution
#   cp dci_IPoverWDM_request_upper_bound_solution.temp dci_customer_request_solution.temp


fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2 $3 $2 > dci_lightpath_request_upper_bound_solution.temp
#   cp 0_requests dci_IPoverWDM_request_upper_bound_solution.temp
#   cp dci_lightpath_request_upper_bound_solution.temp  $1.output.dci_request_solution
#   cp dci_lightpath_request_upper_bound_solution.temp dci_customer_request_upper_bound_solution.temp
fi
#########

cp dci_lead_time_checklist.temp dci_lead_time_checklist.seq_4.temp
cp dci_lead_time_checklist.temp $1.dci_lead_time_checklist.seq_4.temp



rm eee.mps
echo "###########################"
echo $1 leadtime
echo "###########################"

./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -l $2 $Recovery_Task_Per_Unit_Time > model-leadtime.out
cp ee.mps eee.mps
cat model-leadtime.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-leadtime.1
cp  model-leadtime.1 $1.leadtime.seq_4.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.00000000001" "mipopt" "write eee.sol 1" > $1.leadtime.seq_4.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.leadtime.seq_4.mps

cp eee.sol $1.leadtime.seq_4.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.leadtime.seq_4.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-leadtime.1 eee.1 > $1.leadtime.seq_4.sol


cat $1.leadtime.seq_4.sol |grep Qj > Qjg.tmp
wc -l < Qjg.tmp > Qjg_lead_time_solution.temp
cat Qjg.tmp >> Qjg_lead_time_solution.temp
cp Qjg_lead_time_solution.temp Qjg_lead_time_solution.seq_4.temp
cp Qjg_lead_time_solution.temp $1.seq_4.Qjg_lead_time_solution.temp

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp dci_IPoverWDM_request_upper_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_IPoverWDM_request_upper_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_4.temp
   cp dci_lead_time_solution.temp $1.output.dci_IPoverWDM_request_solution.seq_4.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_4

fi


if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp dci_lightpath_request_upper_bound_solution.temp dci_customer_request_solution.temp
   echo "Read Qjg_lead_time_solution.temp."
   echo "Read dci_customer_request_solution.temp."
   echo "Mofigying the leattime info."
 
   ./rcon_milp51_esen_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v2 -t 0 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.temp
   cp dci_lead_time_solution.temp dci_lightpath_request_upper_bound_solution.temp
   cp dci_lead_time_solution.temp dci_customer_request_solution.seq_4.temp
   cp dci_lead_time_solution.temp $1.output.dci_lightpath_request_solution.seq_4.temp
   cp dci_lead_time_solution.temp $1.output.dci_customer_request_solution.seq_4

fi

cp links.csv $1.output.seq_4.links

cp carrier_evaluation.csv $1.output.seq_4.evaluation

rm *.tmp
rm *.mps
rm *.log
rm *.out
rm *.1

#########
# Done
#########

