add_mpi_test(linear_elastic main.cc 1 ${MODEL_DIR}/cube_uniaxial_10kPa.smd  ${MESH_DIR}/cube_24_tets.sms)
target_link_libraries(linear_elastic util meta interface)

#add_test(NAME linear_elastic_regression
#         COMMAND ${CMAKE_COMMAND} -E compare_files "${CMAKE_CURRENT_SOURCE_DIR}/output" "${CMAKE_CURRENT_BINARY_DIR}/output")

add_mpi_test(partitioned_linear_elastic main.cc 2 ${MODEL_DIR}/cube_uniaxial_10kPa.smd ${MESH_DIR}/cube_24_tets_2_parts.sms)
target_link_libraries(partitioned_linear_elastic util meta interface)

file(COPY        ${CMAKE_CURRENT_SOURCE_DIR}/petsc_options
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)