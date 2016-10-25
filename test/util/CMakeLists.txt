add_mpi_test(util_init init/test.cc 1)
target_link_libraries(util_init util)

execute_process(COMMAND tty OUTPUT_VARIABLE PTS)
string(REGEX REPLACE "\n" "" PTS ${PTS})
add_mpi_test(logging logging/main.cc 1 "${PTS}")
target_link_libraries(logging util)

#add_mpi_test(byte_packing byte_packing/test.cc 1)
#target_link_libraries(byte_packing util)
