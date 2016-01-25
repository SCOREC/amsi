add_mpi_test(reciprocal_comm test.cc 8 -a amsi_options_3_5)
target_link_libraries(reciprocal_comm util meta interface control)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsi_options_3_5
    ${CMAKE_CURRENT_BINARY_DIR}/amsi_options_3_5 COPYONLY)
