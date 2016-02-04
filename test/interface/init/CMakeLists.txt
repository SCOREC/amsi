add_mpi_test(interface_init test.cc 1)
target_link_libraries(interface_init util meta interface)

file(COPY        ${CMAKE_CURRENT_SOURCE_DIR}/petsc_options
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)