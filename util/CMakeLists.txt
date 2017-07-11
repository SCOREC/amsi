set(util_public_headers
  amsiByteStream.h
  amsiCasters.h
  amsiConversionIterator.h
  amsiDistributed.h
  amsiEnumOps.h
  amsiFileSys.h
  amsiUtil.h
  amsiUtilConfig.h
  amsiHost.h
  amsiHost_impl.h
  amsiLocality.h
  amsiObserver.h
  amsiMPI.h
  amsiMPI_impl.h
  amsiOperators.h
  amsiOutput.h
  amsiPArray.h
  amsiPermute.h
  amsiRankSet.h
  amsiRankSet_impl.h
  amsiReporter.h
  amsiString.h
  amsiTee.h
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
  amsiOutput.cc
  amsiPArray.cc
  amsiRankSet.cc
  amsiReporter.cc
  amsiString.cc
  amsiUtil.cc
  amsiUUID.cc
  pystring.cc
  )

find_package(HWLOC REQUIRED)
find_package(SCOREC CONFIG REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h
               @ONLY)

add_library(util ${util_source_files})
set_property(TARGET util PROPERTY VERSION ${AMSI_VERSION})

target_include_directories(util PUBLIC
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                $<INSTALL_INTERFACE:include/amsi>)
target_link_libraries(util PUBLIC SCOREC::pcu PkgConfig::HWLOC)

if("${HOST}" STREQUAL "BGQ")
  target_include_directories(util PUBLIC ${BOOST_INCLUDE_DIR})
endif()

install(TARGETS util
        EXPORT amsiutilTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

# this allows us to use the package from the
# build tree which is wanted for multiple build configs
export(EXPORT amsiutilTargets
       FILE "${PROJECT_BINARY_DIR}/amsi/amsiutilTargets.cmake"
       NAMESPACE amsi::)

install(EXPORT amsiutilTargets
        DESTINATION ${ConfigPackageLocation}
        NAMESPACE amsi::
        )

install(FILES ${util_public_headers} DESTINATION include/amsi)
