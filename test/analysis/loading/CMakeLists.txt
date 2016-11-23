add_mpi_test(loading test.cc 1 ${MODEL_DIR}/cube_uniaxial_10kPa.smd ${MESH_DIR}/cube_24_tets.sms)
target_link_libraries(loading util meta interface)

add_mpi_test(task_loading test2.cc 2 ${MODEL_DIR}/cube_uniaxial_10kPa.smd ${MESH_DIR}/cube_24_tets_2_parts.sms)
target_link_libraries(task_loading util meta interface)