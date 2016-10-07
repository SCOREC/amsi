set(util_public_headers
  amsiByteStream.h
  amsiConversionIterator.h
  amsiDistributed.h
  amsiFileSys.h
  amsiUtil.h
  amsiUtilConfig.h
  amsiHost.h
  amsiHost_impl.h
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
  amsiByteStream.h
  unordered_map.h
  pystring.h
  )

set(util_source_files
  amsiFileSys.cc
  amsiHost.cc
  amsiLocality.cc
  amsiUtil.cc
  amsiMPI.cc
  amsiPArray.cc
  amsiRankSet.cc
  amsiReporter.cc
  amsiUtil.cc
  amsiUUID.cc
  pystring.cc
  )

find_package(hwloc REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h
               @ONLY)

add_library(util ${util_source_files})
set_property(TARGET util PROPERTY VERSION ${AMSI_VERSION})

target_include_directories(util PUBLIC
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                $<INSTALL_INTERFACE:include/amsi>
                                ${HWLOC_INCLUDE_DIR})
target_link_libraries(util PUBLIC ${HWLOC_LIBRARIES})

if("${HOST}" STREQUAL "BGQ")
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
