add_mpi_test(nonlinear_elastic main.cc 1)

target_link_libraries(nonlinear_elastic util meta interface)
