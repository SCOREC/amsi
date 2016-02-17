add_mpi_test(neumann_test test.cc 1 ${MODEL_DIR}/cube_uniaxial_10kPa.smd ${MESH_DIR}/cube_24_tets.sms)
target_link_libraries(dirichlet_test util meta interface)

#add_test(NAME dirichlet_bc_regression
#         COMMAND ${CMAKE_COMMAND} -E compare_files "${CMAKE_CURRENT_SOURCE_DIR}/output" "${CMAKE_CURRENT_BINARY_DIR}/output")
