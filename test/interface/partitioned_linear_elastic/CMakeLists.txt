add_mpi_test(partitioned_linear_elastic main.cc 2 -g ${MODEL_DIR}/cube_uniaxial_10kPa.smd -m ${MESH_DIR}/cube_24_tets_2_parts.sms 2>&1 > output)
target_link_libraries(partitioned_linear_elastic util meta interface)

#add_test(NAME partitioned_linear_elastic_regression
#         COMMAND ${CMAKE_COMMAND} -E compare_files "${CMAKE_CURRENT_SOURCE_DIR}/output" "${CMAKE_CURRENT_BINARY_DIR}/output")

file(COPY        ${CMAKE_CURRENT_SOURCE_DIR}/petsc_options
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
