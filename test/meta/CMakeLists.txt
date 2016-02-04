add_mpi_test(meta_init init/test.cc 1)
target_link_libraries(meta_init util meta)

add_mpi_test(reconcile_plan reconcile_plan/test.cc 1)
target_link_libraries(reconcile_plan util meta)

add_mpi_test(process_set process_set/test.cc 1)
target_link_libraries(process_set util meta)

add_mpi_test(data_distribution data_distribution/test.cc 1)
target_link_libraries(data_distribution util meta)

add_mpi_test(task task/test.cc 1)
target_link_libraries(task util meta)

add_mpi_test(scale scale/test1.cc 32)
target_link_libraries(scale util meta)

add_mpi_test(task_manager task_manager/test.cc 32)
target_link_libraries(task_manager util meta)

add_mpi_test(communication_manager communication_manager/test.cc 32)
target_link_libraries(communication_manager util meta)
