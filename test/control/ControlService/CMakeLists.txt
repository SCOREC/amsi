add_mpi_test(control_service test.cc 32 -a amsi_options_8_24)
target_link_libraries(control_service util meta interface control)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/amsi_options_8_24
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
