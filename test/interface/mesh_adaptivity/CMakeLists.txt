add_mpi_test(uniform_adapt uniform.cc 1 ${MESH_DIR}/cube_24_tets.sms)
target_link_libraries(uniform_adapt util meta interface)

add_mpi_test(error_estimator error_estimator.cc 1 ${MODEL_DIR}/cube_uniaxial_10kPa.smd ${MESH_DIR}/cube_24_tets.sms)
target_link_libraries(error_estimator util meta interface)

file(COPY        ${CMAKE_CURRENT_SOURCE_DIR}/petsc_options
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)