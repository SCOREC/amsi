add_mpi_test(uniform_adapt uniform.cc 1)
target_link_libraries(uniform_adapt util meta interface)

add_mpi_test(error_estimator error_estimator.cc 1)
target_link_libraries(error_estimator util meta interface)
