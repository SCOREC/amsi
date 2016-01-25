set(util_public_headers
  amsiByteStream.h
  amsiDistributed.h
  amsiFail.h
  amsiFileSys.h
  amsiUtil.h
  amsiUtilConfig.h
  amsiHost.h
  amsiLocality.h
  amsiObserver.h
  amsiMPI.h
  amsiMPI_impl.h
  amsiPArray.h
  amsiPermute.h
  amsiRankSet.h
  amsiRankSet_impl.h
  amsiReporter.h
  amsiUtilFunctions.h
  amsiUUID.h
  unordered_map.h
  pystring.h
  )

set(util_source_files
  amsiFileSys.cc
  amsiLocality.cc
  amsiMPI.cc
  amsiPArray.cc
  amsiRankSet.cc
  amsiReporter.cc
  amsiUtil.cc
  amsiUUID.cc
  pystring.cc
  )

find_package(hwloc REQUIRED)
find_package(Core COMPONENTS Sim)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h
               @ONLY)

add_library(util ${util_source_files})
set_property(TARGET util PROPERTY VERSION ${AMSI_VERSION})

target_include_directories(util PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/amsi>
  ${CORE_INCLUDE_DIRS}
  ${HWLOC_INCLUDE_DIR})

target_link_libraries(util PUBLIC
 ${CORE_LIBS}
 ${HWLOC_LIBRARIES})

if($ENV{HOST} STREQUAL "q.ccni.rpi.edu")
  target_include_directories(util PUBLIC ${BOOST_INCLUDE_DIR})
endif()

install(TARGETS util
        EXPORT amsiutilTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

install(EXPORT amsiutilTargets
        DESTINATION ${ConfigPackageLocation})

install(FILES ${util_public_headers} DESTINATION include/amsi)
