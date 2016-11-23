add_mpi_test(volume volume.cc 2 ${MODEL_DIR}/neuron_with_cell.smd ${MESH_DIR}/neuron_with_cell_30k_2_parts.sms 2)
target_link_libraries(volume util meta interface)