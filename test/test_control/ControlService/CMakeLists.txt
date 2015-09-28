add_mpi_test(control_service test.cc 32 "-a amsi_options_8_24")
target_link_libraries(control_service amsi_util amsi_meta amsi_interface amsi_control)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsi_options_8_24
    ${CMAKE_CURRENT_BINARY_DIR}/amsi_options_8_24 COPYONLY)
