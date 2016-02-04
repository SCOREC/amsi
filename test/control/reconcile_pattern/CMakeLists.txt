add_mpi_test(reconcile_pattern test.cc 32 -a amsi_options_8_24)
target_link_libraries(reconcile_pattern util meta interface control)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/amsi_options_8_24
     DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/)