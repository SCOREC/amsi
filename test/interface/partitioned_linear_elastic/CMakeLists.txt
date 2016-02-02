add_mpi_test(partitioned_linear_elastic main.cc 1)
# would prefer to only depent on interface, but init/free functions are in control
target_link_libraries(partitioned_linear_elastic util meta interface)

file(COPY        ${CMAKE_CURRENT_SOURCE_DIR}/petsc_options
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
