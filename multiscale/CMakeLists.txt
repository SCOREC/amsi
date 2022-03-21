
set(multiscale_public_headers
  amsiMultiscale.h
  amsiAssemblable.h
  amsiAddData.h
  amsiCommunicationManager.h
  amsiControlService.h
  amsiControlService_impl.h
  amsiCoupling.h
  amsiCouplingData.h
  amsiCreateCoupling.h
  amsiDataDistribution.h
  amsiMigration.h
  amsiPlanMigration.h
  amsiProcessSet.h
  amsiProcessAllocator.h
  amsiScale.h
  amsiTask.h
  amsiTaskGroup.h
  amsiTaskManager.h
  )

set(multiscale_source_files
  amsiMultiscale.cc
  amsiAssemblable.cc
  amsiAddData.cc
  amsiCommunicationManager.cc
  amsiControlService.cc
  amsiCoupling.cc
  amsiCreateCoupling.cc
  amsiDataDistribution.cc
  amsiMigration.cc
  amsiPlanMigration.cc
  amsiProcessSet.cc
  amsiScale.cc
  amsiTask.cc
  amsiTaskGroup.cc
  amsiTaskManager.cc
  )

add_library(multiscale ${multiscale_source_files})
add_library(amsi::multiscale ALIAS multiscale)

target_include_directories(multiscale PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/amsi>)
target_link_libraries(multiscale PUBLIC util)


option(ENABLE_ZOLTAN "Enable Zoltan Support" ON)
if (ENABLE_ZOLTAN)
  #find_package(ZOLTAN REQUIRED)
  find_package(Zoltan REQUIRED)
  target_link_libraries(multiscale PUBLIC ${ZOLTAN_LIBRARIES})
  target_include_directories(multiscale PUBLIC ${ZOLTAN_INCLUDE_DIRS})
  target_compile_definitions(multiscale PUBLIC -DZOLTAN)
endif()

set_property(TARGET multiscale PROPERTY VERSION ${AMSI_VERSION})

install(TARGETS multiscale
        EXPORT amsimultiscaleTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

# this allows us to use the package from the
# build tree which is wanted for multiple build configs
export(EXPORT amsimultiscaleTargets
       FILE "${PROJECT_BINARY_DIR}/amsi/amsimultiscaleTargets.cmake"
       NAMESPACE amsi::)

install(EXPORT amsimultiscaleTargets
        DESTINATION ${ConfigPackageLocation}
        NAMESPACE amsi::)

install(FILES ${multiscale_public_headers} DESTINATION include/amsi)

# Headers need to be copied to binary directory to enable building against build
# W/O installation
file(COPY ${multiscale_public_headers} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
