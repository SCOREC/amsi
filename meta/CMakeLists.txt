set(meta_public_headers
  amsiMeta.h
  amsiMetaConfig.h
  amsiAddData.h
  amsiCoupling.h
  amsiCouplingData.h
  amsiCreateCoupling.h
  amsiScale.h
  ProcessSet.h
  ProcessAllocator.h
  DataDistribution.h
  Task.h
  TaskGroup.h
  TaskManager.h
  CommunicationManager.h
  PlanMigration.h
  Assemblable.h
  )

set(meta_source_files
  amsiMeta.cc
  amsiAddData.cc
  amsiCoupling.cc
  amsiCreateCoupling.cc
  amsiScale.cc
  ProcessSet.cc
  DataDistribution.cc
  Task.cc
  TaskGroup.cc
  TaskManager.cc
  CommunicationManager.cc
  PlanMigration.cc
  Assemblable.cc
  )

find_package(zoltan)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiMetaConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiMetaConfig.h
               @ONLY)

add_library(meta ${meta_source_files})

target_include_directories(meta PUBLIC
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                $<INSTALL_INTERFACE:include/amsi>)
target_link_libraries(meta PUBLIC util)

if(ZOLTAN_FOUND)
  target_link_libraries(meta PUBLIC ${ZOLTAN_LIBRARIES})
  target_include_directories(meta PUBLIC ${ZOLTAN_INCLUDE_DIRS})
endif()

set_property(TARGET meta PROPERTY VERSION ${AMSI_VERSION})

install(TARGETS meta
        EXPORT amsimetaTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

install(EXPORT amsimetaTargets
        DESTINATION ${ConfigPackageLocation})

install(FILES ${meta_public_headers} DESTINATION include/amsi)
