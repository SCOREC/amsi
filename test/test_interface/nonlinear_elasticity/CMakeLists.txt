add_mpi_test(nonlinear_elastic main.cc 1)

target_link_libraries(nonlinear_elastic amsi_util amsi_meta amsi_interface)
