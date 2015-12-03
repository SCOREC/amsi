set(meta_public_headers 
  amsiMeta.h
  amsiMetaConfig.h
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

find_package(PETSc)
find_package(zoltan)
find_package(Core COMPONENTS Sim)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiMetaConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiMetaConfig.h
               @ONLY)

add_library(amsi_meta ${meta_source_files})

target_include_directories(amsi_meta PUBLIC
				     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
				     $<INSTALL_INTERFACE:include/amsi>
                                     ${CORE_INCLUDE_DIRS})
target_link_libraries(amsi_meta PUBLIC amsi_util ${CORE_LIBS})

if(ZOLTAN_FOUND)
  target_link_libraries(amsi_meta PUBLIC ${ZOLTAN_LIBRARIES})
  target_include_directories(amsi_meta PUBLIC ${ZOLTAN_INCLUDE_DIRS})
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
