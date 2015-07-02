add_mpi_test(partitioned_linear_elastic main.cc 1)

# would prefer to only depent on amsi_interface, but init/free functions are in amsi_control
target_link_libraries(partitioned_linear_elastic amsi_util amsi_meta amsi_interface amsi_control) 
