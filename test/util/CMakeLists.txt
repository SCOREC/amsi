configure_file(${CMAKE_CURRENT_SOURCE_DIR}/init/amsi_options
               ${CMAKE_CURRENT_BINARY_DIR}/amsi_options COPYONLY)
add_mpi_test(util_init init/test.cc 1 -a amsi_options)
target_link_libraries(util_init util)

execute_process(COMMAND tty OUTPUT_VARIABLE PTS)
string(REGEX REPLACE "\n" "" PTS ${PTS})
add_mpi_test(util_logging logging/main.cc 1 "${PTS}")
target_link_libraries(util_logging util)

add_mpi_test(byte_packing byte_packing/test.cc 1)
target_link_libraries(byte_packing util)
