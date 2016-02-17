add_mpi_test(dirichlet_test test.cc 1 -g ${MODEL_DIR}/cube_uniaxial_10kPa.smd -m ${MESH_DIR}/cube_24_tets.sms -d ${CMAKE_CURRENT_SOURCE_DIR}/dirichlet_spec.txt 2>&1 > output)
target_link_libraries(dirichlet_test util meta interface)

#add_test(NAME dirichlet_bc_regression
#         COMMAND ${CMAKE_COMMAND} -E compare_files "${CMAKE_CURRENT_SOURCE_DIR}/output" "${CMAKE_CURRENT_BINARY_DIR}/output")

file(COPY        ${CMAKE_CURRENT_SOURCE_DIR}/petsc_options
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
