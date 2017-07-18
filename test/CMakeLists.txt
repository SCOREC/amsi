
set(test_header_dir ${CMAKE_CURRENT_SOURCE_DIR})
function(add_mpi_test name file no_mpi_proc)
  message(STATUS "Adding test " ${name})
  add_executable(${name} ${file})
  target_include_directories(${name} PUBLIC ${test_header_dir})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    get_target_property(FLGS ${name} LINK_FLAGS_DEBUG)
    if(FLGS STREQUAL "FLGS-NOTFOUND")
      SET(FLGS "") # set to empty string
    endif()
    SET(FLGS "${FLGS} ${GCC_COVERAGE_LINK_FLAGS}" )
    set_target_properties(${name} PROPERTIES LINK_FLAGS_DEBUG ${FLGS} )
  endif()
  set(test_parameters -np ${no_mpi_proc} ${name} ${ARGN})
  add_test(NAME ${name} COMMAND "mpirun" ${test_parameters} )
endfunction(add_mpi_test)

# add test that runs with mpi and links against google-test
function(add_mpi_gtest name file no_mpi_proc)
  message(STATUS "Adding test " ${name})
  add_executable(${name} ${file})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    get_target_property(FLGS ${name} LINK_FLAGS_DEBUG)
    if(FLGS STREQUAL "FLGS-NOTFOUND")
      SET(FLGS "") # set to empty string
    endif()
    SET(FLGS "${FLGS} ${GCC_COVERAGE_LINK_FLAGS}" )
    set_target_properties(${name} PROPERTIES LINK_FLAGS_DEBUG ${FLGS} )
  endif()
  target_link_libraries(${name} gtest_main}
  set(test_parameters -np ${no_mpi_proc} ${name} ${ARGN})
  add_test(NAME ${name} COMMAND "mpirun" ${test_parameters} )
endfunction(add_mpi_test)

add_subdirectory(util)
add_subdirectory(multiscale)
add_subdirectory(analysis)
add_subdirectory(multiscale_analysis)
