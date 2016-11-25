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

add_mpi_test(cntrl control_service/test.cc 32 -a control_service/amsi_options_8_24)
target_link_libraries(cntrl util meta)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/control_service/amsi_options_8_24
               ${CMAKE_CURRENT_BINARY_DIR}/control_service/amsi_options_8_24 COPYONLY)

add_mpi_test(rcnl_ptrn reconcile_pattern/test.cc 32 -a reconcile_pattern/amsi_options_8_24)
target_link_libraries(rcnl_ptrn util meta)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/reconcile_pattern/amsi_options_8_24
               ${CMAKE_CURRENT_BINARY_DIR}/reconcile_pattern/amsi_options_8_24 COPYONLY)

add_mpi_test(rcprcl_cmm recip_comm/test.cc 8 -a recip_comm/amsi_options_3_5)
target_link_libraries(rcprcl_cmm util meta)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/recip_comm/amsi_options_3_5
               ${CMAKE_CURRENT_BINARY_DIR}/recip_comm/amsi_options_3_5 COPYONLY)