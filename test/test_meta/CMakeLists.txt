add_mpi_test(process_set ProcessSet/test.cc 1)
target_link_libraries(process_set amsi_util amsi_meta)

add_mpi_test(data_distribution DataDistribution/test.cc 1)
target_link_libraries(data_distribution amsi_util amsi_meta)

add_mpi_test(task Task/test.cc 1)
target_link_libraries(task amsi_util amsi_meta)

add_mpi_test(task_manager TaskManager/test.cc 32)
target_link_libraries(task_manager amsi_util amsi_meta)

add_mpi_test(communication_manager CommunicationManager/test.cc 32)
target_link_libraries(communication_manager amsi_util amsi_meta)
