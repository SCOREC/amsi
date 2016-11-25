find_package(zoltan)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiMultiscaleConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiMultiscaleConfig.h
               @ONLY)

set(multiscale_public_headers
  amsiMultiscale.h
  amsiMultiscaleConfig.h
  amsiAddData.h
  amsiControlService.h
  amsiControlService_impl.h
  amsiCoupling.h
  amsiCouplingData.h
  amsiCreateCoupling.h
  amsiScale.h
  amsiProcessSet.h
  amsiProcessAllocator.h
  amsiDataDistribution.h
  amsiTask.h
  amsiTaskGroup.h
  amsiTaskManager.h
  amsiCommunicationManager.h
  amsiPlanMigration.h
  amsiAssemblable.h
  )

set(multiscale_source_files
  amsiMultiscale.cc
  amsiAddData.cc
  amsiControlService.cc
  amsiCoupling.cc
  amsiCreateCoupling.cc
  amsiScale.cc
  amsiProcessSet.cc
  amsiDataDistribution.cc
  amsiTask.cc
  amsiTaskGroup.cc
  amsiTaskManager.cc
  amsiCommunicationManager.cc
  amsiPlanMigration.cc
  amsiAssemblable.cc
  )

add_library(multiscale ${multiscale_source_files})

target_include_directories(multiscale PUBLIC
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                $<INSTALL_INTERFACE:include/amsi>)
target_link_libraries(multiscale PUBLIC util)

if(ZOLTAN_FOUND)
  target_link_libraries(multiscale PUBLIC ${ZOLTAN_LIBRARIES})
  target_include_directories(multiscale PUBLIC ${ZOLTAN_INCLUDE_DIRS})
endif()

set_property(TARGET multiscale PROPERTY VERSION ${AMSI_VERSION})

install(TARGETS multiscale
        EXPORT amsimultiscaleTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

install(EXPORT amsimultiscaleTargets
        DESTINATION ${ConfigPackageLocation})

install(FILES ${multiscale_public_headers} DESTINATION include/amsi)
