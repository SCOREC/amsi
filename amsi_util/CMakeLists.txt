find_package(MPI REQUIRED)
find_package(PETSc)
find_package(ZOLTAN) # only here for amsiConfig.h.in configuration
find_package(CoreSim)
if(NOT CORE_FOUND)
  find_package(Core)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiConfig.h.in 
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiConfig.h 
	       @ONLY)

set(util_public_headers
  amsiUtil.h
  amsiConfig.h
  amsiReporter.h
  amsiUtilFunctions.h
  amsiMPI.h
  amsiByteStream.h
  unordered_map.h 
  pystring.h
  )

#really don't want unordered map to be public... hmm

set(util_source_files
  amsiUtil.cc
  amsiReporter.cc
  pystring.cc
  )

add_library(amsi_util ${util_source_files} ${util_public_headers})
set_property(TARGET amsi_util PROPERTY VERSION ${AMSI_VERSION})

target_include_directories(amsi_util PUBLIC
				     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
				     $<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(amsi_util PUBLIC ${MPI_LIBRARIES})

if(PETSC_FOUND)
  target_include_directories(amsi_util PUBLIC ${PETSC_INCLUDES})
endif()

if(CORE_FOUND)
  target_include_directories(amsi_util PUBLIC ${CORE_INCLUDE_DIRS})
endif()

install(TARGETS amsi_util 
	EXPORT amsiutilTargets 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include/amsi)

install(EXPORT amsiutilTargets
	DESTINATION ${ConfigPackageLocation})

install(FILES ${util_public_headers} DESTINATION include/amsi)
