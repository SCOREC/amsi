

add_mpi_test(linear_elastic main.cc 1)
target_link_libraries(linear_elastic amsi_util amsi_meta amsi_interface) 
