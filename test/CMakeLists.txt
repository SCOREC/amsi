include(CTest)

# testing
function(add_mpi_test name file no_mpi_proc)
  add_executable(${name} ${file})
  set(test_parameters -np ${no_mpi_proc} ${name} ${ARGN})
  add_test(NAME ${name} COMMAND "mpirun" ${test_parameters} )
endfunction(add_mpi_test)

add_subdirectory(util)
add_subdirectory(meta)
add_subdirectory(interface)
add_subdirectory(control)

#add_mpi_test(processSet test/ProcessSet/test.cc 1)
#add_mpi_test(dataDistribution test/DataDistribution/test.cc 1)
#add_mpi_test(task test/Task/test.cc 1)
#add_mpi_test(taskManager test/TaskManager/test.cc 32)
#add_mpi_test(communicationManager test/CommunicationManager/test.cc 32)
#add_mpi_test(controlService test/ControlService/test.cc 32)
#add_mpi_test(reciprocalComm test/ReciprocalComm/test.cc 8)
#add_mpi_test(mpiT test/MPI/test.cc 1)
