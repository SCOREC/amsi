add_mpi_test(control_init test.cc 1)
target_link_libraries(control_init util meta interface control)