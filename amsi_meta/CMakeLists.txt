find_package(MPI REQUIRED)
find_package(ZOLTAN)
find_package(PETSc)

set(meta_public_headers 
  amsiMeta.h
  ProcessSet.h
  DataDistribution.h
  Task.h
  TaskGroup.h
  TaskManager.h
  CommunicationManager.h
  CommPattern.h
  CommPattern_Create.h
  AddData_LoadBalancing.h
  PlanMigration.h
  Assemblable.h
  )

set(meta_source_files 
  amsiMeta.cc
  ProcessSet.cc
  DataDistribution.cc
  Task.cc
  TaskGroup.cc
  TaskManager.cc
  CommunicationManager.cc
  CommPattern.cc
  CommPattern_Create.cc
  AddData_LoadBalancing.cc
  PlanMigration.cc
  )

add_library(amsi_meta ${meta_source_files} ${meta_public_headers})

target_include_directories(amsi_meta PUBLIC
				     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
				     $<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(amsi_meta PUBLIC amsi_util ${MPI_LIBRARIES})

if(ZOLTAN_FOUND)
  target_link_libraries(amsi_meta PUBLIC ${ZOLTAN_LIBRARIES})
  target_include_directories(amsi_meta PUBLIC ${ZOLTAN_INCLUDE_DIRS})
endif()

if(PETSC_PARMETIS)
  target_link_libraries(amsi_meta PUBLIC ${PETSC_LIBRARIES})
endif()

if(CORE_FOUND)
  target_link_libraries(amsi_meta PUBLIC ${CORE_LIBRARIES})
endif()

set_property(TARGET amsi_meta PROPERTY VERSION ${AMSI_VERSION})

install(TARGETS amsi_meta 
	EXPORT amsimetaTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include/amsi)

install(EXPORT amsimetaTargets
	DESTINATION ${ConfigPackageLocation})

install(FILES ${meta_public_headers} DESTINATION include/amsi)
