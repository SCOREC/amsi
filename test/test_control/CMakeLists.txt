add_mpi_test(control_service ControlService/test.cc 32)
target_link_libraries(control_service amsi_util amsi_meta amsi_interface amsi_control)

add_mpi_test(reciprocal_comm ReciprocalComm/test.cc 8)
target_link_libraries(reciprocal_comm amsi_util amsi_meta amsi_interface amsi_control)

