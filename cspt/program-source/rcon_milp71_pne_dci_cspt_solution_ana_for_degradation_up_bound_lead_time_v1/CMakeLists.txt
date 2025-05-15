cmake_minimum_required(VERSION 3.0)

project(rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1)

add_executable(rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1 main.cpp)

include_directories (/usr/local/include)
link_directories (/usr/local/lib)
set (LIBS ${LIBS} -lpthread)

target_link_libraries(rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1 ${LIBS})

#install(TARGETS rcon_milp71_pne_dci_cspt_solution_ana_for_degradation_up_bound_lead_time_v1 RUNTIME DESTINATION bin)
