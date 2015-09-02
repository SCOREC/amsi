set(meta_public_headers 
  amsiMeta.h
  ProcessSet.h
  ProcessAllocator.h
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

find_package(MPI REQUIRED)
find_package(CoreSim REQUIRED)
find_package(PETSc)
find_package(ZOLTAN REQUIRED)

add_library(amsi_meta ${meta_source_files} ${meta_public_headers})

target_include_directories(amsi_meta PUBLIC
				     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
				     $<INSTALL_INTERFACE:include/amsi>)

if(PETSC_PARMETIS)
  target_link_libraries(amsi_meta PUBLIC ${PETSC_LIBRARIES})
  target_include_directories(amsi_meta PUBLIC ${PETSC_INCLUDES})  
endif()

if(CORE_FOUND)
  target_link_libraries(amsi_meta PUBLIC ${CORE_LIBRARIES})
  target_include_directories(amsi_meta PUBLIC ${CORE_INCLUDE_DIRS})
endif()

target_link_libraries(amsi_meta PUBLIC amsi_util ${ZOLTAN_LIBRARIES} ${MPI_C_LIBRARIES})
target_include_directories(amsi_meta PUBLIC ${ZOLTAN_INCLUDE_DIRS} ${MPI_C_INCLUDE_PATH})

set_property(TARGET amsi_meta PROPERTY VERSION ${AMSI_VERSION})

install(TARGETS amsi_meta 
	EXPORT amsimetaTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include/amsi)

install(EXPORT amsimetaTargets
	DESTINATION ${ConfigPackageLocation})

install(FILES ${meta_public_headers} DESTINATION include/amsi)
