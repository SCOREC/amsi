add_mpi_test(uniform_adapt uniform.cc 1)
target_link_libraries(uniform_adapt amsi_util amsi_meta amsi_interface) 

add_mpi_test(error_estimator error_estimator.cc 1)
target_link_libraries(error_estimator amsi_util amsi_meta amsi_interface)
