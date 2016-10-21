include(CTest)

set(test_header_dir ${CMAKE_CURRENT_SOURCE_DIR})
function(add_mpi_test name file no_mpi_proc)
  message(STATUS "Adding test " ${name})
  add_executable(${name} ${file})
  target_include_directories(${name} PUBLIC ${test_header_dir})
  get_target_property(FLGS ${name} LINK_FLAGS_DEBUG)
  if(FLGS STREQUAL "FLGS-NOTFOUND")
    SET(FLGS "") # set to empty string
  endif()
  SET(FLGS "${FLGS} ${GCC_COVERAGE_LINK_FLAGS}" )
  set_target_properties(${name} PROPERTIES LINK_FLAGS_DEBUG ${FLGS} )
  set(test_parameters -np ${no_mpi_proc} ${name} ${ARGN})
  add_test(NAME ${name} COMMAND "mpirun" ${test_parameters} )
endfunction(add_mpi_test)

add_subdirectory(util)
add_subdirectory(meta)
add_subdirectory(interface)
add_subdirectory(control)

