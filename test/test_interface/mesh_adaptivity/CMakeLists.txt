add_mpi_test(uniform_adapt uniform.cc 1)

# would prefer to only depent on amsi_interface, but init/free functions are in amsi_control
target_link_libraries(uniform_adapt amsi_util amsi_meta amsi_interface amsi_control) 
