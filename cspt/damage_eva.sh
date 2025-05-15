Recovery_Task_Per_Unit_Time=1

#cp carrier_$2.original_requests original_requests.txt


tt=`date`

echo "###########################"
echo Carrier $2 instance $1 $3 Start TIME: $tt
echo "###########################"


if [ "$2" = "0" ]
then
   echo "For carrier 0 damage evaluation."
   cp carrier_0.input.request original_requests.txt
fi

if [ "$2" = "1" ]
then
   echo "For carrier 1 damage evaluation."
   cp carrier_1.input.request original_requests.txt
fi

if [ "$2" = "2" ]
then
   echo "For carrier 2 damage evaluation."
   cp carrier_2.input.request original_requests.txt
fi


rm conf.txt
rm lightpath_price.txt
rm customer_lightpath_requests.txt
rm carrier_lightpath_requests.txt
rm connection_requests.txt

cp $1.input.conf conf.txt
cp $1.input.price lightpath_price.txt
cp 0_requests carrier_lightpath_requests.txt
#cp $1.input.carrier_lightpath_request carrier_lightpath_requests.txt


if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp $1.input.request connection_requests.txt
   cp 0_requests customer_lightpath_requests.txt
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp $1.input.request customer_lightpath_requests.txt
   cp 0_requests connection_requests.txt
fi


tt=`date`

rm ee.mps
echo "###########################"
echo Carrier $2 instance $1 $3  Solving request TIME: $tt
echo "###########################"

./rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_low_bound_v1 -m $2 > model.out

#old version ./rcon_milp41_esen_dci_cspt_v3  -m $2 > model.out
cat model.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model.1
cp  model.1 $1.lower_bound.mod

rm ee.sol
~/cplex -c "read ee.mps" "set timelimit 7200" "set parallel -1" "set threads 40" "set mip tol mipgap 0.0000000000001" "mipopt" "write ee.sol 1" > $1.lower_bound.cplex.log
less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp ee.mps $1.lower_bound.mps

cp ee.sol $1.lower_bound.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat ee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.lower_bound.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model.1 ee.1 > $1.lower_bound.sol


cat $1.lower_bound.sol |grep Bm > Bmn.tmp
wc -l < Bmn.tmp > Bmn_solution.temp
cat Bmn.tmp >> Bmn_solution.temp
cp Bmn_solution.temp $1.lower_bound.Bmn_solution.temp


cat $1.lower_bound.sol |grep Ca > Csd.tmp
wc -l < Csd.tmp > Csd_solution.temp
cat Csd.tmp >> Csd_solution.temp
cp Csd_solution.temp $1.lower_bound.Csd_solution.temp

cat $1.lower_bound.sol |grep Ps > Pmnijw.tmp
wc -l < Pmnijw.tmp > Pmnijw_solution.temp
cat Pmnijw.tmp >> Pmnijw_solution.temp
cp Pmnijw_solution.temp $1.lower_bound.Pmnijw_solution.temp

cat $1.lower_bound.sol |grep Mi > Mijmn.tmp
wc -l < Mijmn.tmp > Mijmn_solution.temp
cat Mijmn.tmp >> Mijmn_solution.temp
cp Mijmn_solution.temp $1.lower_bound.Mijmn_solution.temp

cat $1.lower_bound.sol |grep Xa > Xijsd.tmp
wc -l < Xijsd.tmp > Xijsd_solution.temp
cat Xijsd.tmp >> Xijsd_solution.temp
cp Xijsd_solution.temp $1.lower_bound.Xijsd_solution.temp

cat $1.lower_bound.sol |grep Ub > Ub.tmp
wc -l < Ub.tmp > Ub_solution.temp
cat Ub.tmp >> Ub_solution.temp
cp Ub_solution.temp  $1.lower_bound.Ub_solution.temp

cat $1.lower_bound.sol |grep Vz > Vijw.tmp
wc -l < Vijw.tmp > Vijw_solution.temp
cat Vijw.tmp >> Vijw_solution.temp
cp Vijw_solution.temp $1.lower_bound.Vijw_solution.temp

cat $1.lower_bound.sol |grep Oi > Oija.tmp
wc -l < Oija.tmp > Oija_solution.temp
cat Oija.tmp >> Oija_solution.temp
cp Oija_solution.temp $1.lower_bound.Oija_solution.temp

cat $1.lower_bound.sol |grep Di > Dij.tmp
wc -l < Dij.tmp > Dij_solution.temp
cat Dij.tmp >> Dij_solution.temp
cp Dij_solution.temp $1.lower_bound.Dij_solution.temp



if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_low_bound_v1 $3 $2 > dci_IPoverWDM_request_lower_bound_solution.temp
   cp 0_requests dci_lightpath_request_lower_bound_solution.temp
fi

if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   ./rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_low_bound_v1 $3 $2 > dci_lightpath_request_lower_bound_solution.temp
   cp 0_requests dci_IPoverWDM_request_lower_bound_solution.temp

fi
cp dci_IPoverWDM_request_lower_bound_solution.temp $1.dci_IPoverWDM_request_lower_bound_solution.before-leadtime.lower_bound.temp
cp dci_lightpath_request_lower_bound_solution.temp $1.dci_lightpath_request_lower_bound_solution.before-leadtime.lower_bound.temp


cp dci_lead_time_checklist.temp dci_lead_time_checklist.lower_bound.temp
cp dci_lead_time_checklist.temp $1.dci_lead_time_checklist.lower_bound.temp


#cp links.csv $1.output.links_lower_bound

#cp carrier_evaluation.csv $1.output.evaluation_lower_bound

rm *.tmp
#########

tt=`date`

rm eee.mps
echo "###########################"
echo Carrier $2 instance $1 $3 leadtime TIME: $tt
echo "###########################"

./rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1 -l $2 $Recovery_Task_Per_Unit_Time > model-leadtime.out
cp ee.mps eee.mps
cat model-leadtime.out |grep VARS |sed s/VARS://g |awk '{print $1, $2;}' > model-leadtime.1
cp  model-leadtime.1 $1.leadtime.lower_bound.mod

rm eee.sol
~/cplex -c "read eee.mps" "set timelimit 7200" "set mip tol mipgap 0.00000000001" "mipopt" "write eee.sol 1" > $1.leadtime.lower_bound.cplex.log
less eee.sol |grep variable |awk '{print; if($1== "</variables>") exit}'|awk '{if($4 != "value=\"0\"/>" && $4 != "value=\"-0\"/>" ) print;}'|cut -d"\"" -f2,6 |sed s/\"/\ /g > eee.1

#less ee.sol |grep variable |awk '{print; if($1== "</variables>") exit}' |grep "value=\"1\""|cut -d"\"" -f2,6 |sed s/\"/\ /g > ee.1

cp eee.mps $1.leadtime.lower_bound.mps

cp eee.sol $1.leadtime.lower_bound.result
#cat result |grep value |awk -v "val=$filename.txt $nodepair" '{print val, $5}' > $1.val

cat eee.sol |grep objectiveValue |head -1 |cut -d"\"" -f 2 |awk -v "val=$1" '{print val, $1}' > $1.leadtime.lower_bound.val
#
#sh checkresults.sh > ee.1
awk 'NR==FNR { n[$1] = $2; next } ($1 in n) {print $1, n[$1], $2}' model-leadtime.1 eee.1 > $1.leadtime.lower_bound.sol


cat $1.leadtime.lower_bound.sol |grep Qj > Qjg.tmp
wc -l < Qjg.tmp > Qjg_lead_time_solution.temp
cat Qjg.tmp >> Qjg_lead_time_solution.temp
cp Qjg_lead_time_solution.temp Qjg_lead_time_solution.lower_bound.temp
cp Qjg_lead_time_solution.temp $1.lower_bound.Qjg_lead_time_solution.temp

if [ "$3" = "-e" ]
then
   echo "Use IP over WDM to create connection for Users' (e.g., DCP) traffic."
   cp dci_IPoverWDM_request_lower_bound_solution.temp dci_customer_request_solution.temp
  # echo "Read Qjg_lead_time_solution.temp."
  # echo "Read dci_customer_request_solution.temp."
  # echo "Mofigying the leattime info."
 
   ./rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1 -t $2 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.lower_bound.temp
   cp dci_lead_time_solution.lower_bound.temp dci_IPoverWDM_request_lower_bound_solution.temp
   cp dci_lead_time_solution.lower_bound.temp dci_customer_request_solution.lower_bound.temp
   cp dci_lead_time_solution.lower_bound.temp $1.output.dci_IPoverWDM_request_solution.lower_bound.temp
   cp dci_lead_time_solution.lower_bound.temp $1.output.dci_customer_request_solution.lower_bound

fi


if [ "$3" = "-o" ]
then
   echo "Use lightpath support to create connection for Users' (e.g., DCP) traffic."
   cp dci_lightpath_request_lower_bound_solution.temp dci_customer_request_solution.temp
  # echo "Read Qjg_lead_time_solution.temp."
  # echo "Read dci_customer_request_solution.temp."
  # echo "Mofigying the leattime info."
 
   ./rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1 -t $2 $Recovery_Task_Per_Unit_Time  > dci_lead_time_solution.lower_bound.temp
   cp dci_lead_time_solution.lower_bound.temp dci_lightpath_request_lower_bound_solution.temp
   cp dci_lead_time_solution.lower_bound.temp dci_customer_request_solution.lower_bound.temp
   cp dci_lead_time_solution.lower_bound.temp $1.output.dci_lightpath_request_solution.lower_bound.temp
   cp dci_lead_time_solution.lower_bound.temp $1.output.dci_customer_request_solution.lower_bound

fi


#cp links.csv $1.output.lower_bound.links
#cp carrier_evaluation.csv $1.output.lower_bound.evaluation

cp links.csv $1.output.links
cp carrier_evaluation.csv $1.output.evaluation

tt=`date`

echo "###########################"
echo $1 Done TIME: $tt
echo "###########################"

#########
# Done
#########

