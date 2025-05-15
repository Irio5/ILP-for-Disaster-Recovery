dir=`pwd`

rm -rf 1
rm -rf 2
rm -rf 3
rm -rf 4


#Make base program case gen
#cd $dir/program-source/rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1
#rm -rf build
#mkdir build
#cd build
#cmake ../
#make
#cp rcon_milp71_pne_cases_conf_generation_jpnm_3_carriers_4_dcps_GC2022_Extension_v1 $dir

#Make base program low_bound_v1
cd $dir/program-source/rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_low_bound_v1
rm -rf build
mkdir build
cd build
cmake ../
make
cp rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_low_bound_v1 $dir




#Make base program up_bound_v1
cd $dir/program-source/rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_v1
rm -rf build
mkdir build
cd build
cmake ../
make
cp rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_v1 $dir


#Make base program lead_time_v1
cd $dir/program-source/rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1
rm -rf build
mkdir build
cd build
cmake ../
make
cp rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1 $dir

#Make base program sequential_request_v2
#cd $dir/program-source/rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2/
#rm -rf build
#mkdir build
#cd build
#cmake ../
#make
#cp rcon_milp51_esen_dci_cspt_solution_ana_for_sequential_request_v2  $dir

cd $dir




