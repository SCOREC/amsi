add_mpi_test(multiscale_init init/test.cc 1)
target_link_libraries(multiscale_init util multiscale)

add_mpi_test(multiscale_reconcile_plan reconcile_plan/test.cc 1)
target_link_libraries(multiscale_reconcile_plan util multiscale)

add_mpi_test(multiscale_process_set process_set/test.cc 1)
target_link_libraries(multiscale_process_set util multiscale)

add_mpi_test(multiscale_data_distribution data_distribution/test.cc 1)
target_link_libraries(multiscale_data_distribution util multiscale)

add_mpi_test(multiscale_task task/test.cc 1)
target_link_libraries(multiscale_task util multiscale)

add_mpi_test(multiscale_task_manager task_manager/test.cc 32)
target_link_libraries(multiscale_task_manager util multiscale)

add_mpi_test(multiscale_communication_manager communication_manager/test.cc 32)
target_link_libraries(multiscale_communication_manager util multiscale)

add_mpi_test(multiscale_control_service control_service/test.cc 32)
target_link_libraries(multiscale_control_service util multiscale)

add_mpi_test(multiscale_broadcast control_service/broadcast.cc 32)
target_link_libraries(multiscale_broadcast util multiscale)

add_mpi_test(multiscale_reconcile_pattern reconcile_pattern/test.cc 32)
target_link_libraries(multiscale_reconcile_pattern amsi::amsi util multiscale)

add_mpi_test(multiscale_reciprocal_comm recip_comm/test.cc 8)
target_link_libraries(multiscale_reciprocal_comm util multiscale)