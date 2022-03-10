set(util_public_headers
        amsiByteStream.h
        amsiCasters.h
        amsiConversionIterator.h
        amsiDistributed.h
        amsiEnumOps.h
        amsiExceptions.h
        amsiFail.h
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
  amsiVerbosity.h
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
        pystring.cc)

#find_package(HWLOC REQUIRED)
find_package(SCOREC CONFIG REQUIRED)

# FIXME configured files should be configured into build dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h
        @ONLY)

add_library(util ${util_source_files})
add_library(amsi::util ALIAS util)
set_property(TARGET util PROPERTY VERSION ${AMSI_VERSION})

find_package(MPI REQUIRED)

target_include_directories(util PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/amsi>
        ${HWLOC_INCLUDE_DIRS})
target_link_libraries(util PUBLIC SCOREC::pcu ${HWLOC_LIBRARIES} MPI::MPI_CXX)

set(ENABLE_VERBOSITY HIGH CACHE STRING "Enable Debug/Verbosity Level")
set_property(CACHE ENABLE_VERBOSITY PROPERTY STRINGS HIGH MED LOW OFF)
if (ENABLE_VERBOSITY)
    set(AMSI_VERBOSITY_DEFINITIONS "-DAMSI_VERBOSE_1")
    if (ENABLE_VERBOSITY STREQUAL "LOW")
        set(AMSI_VERBOSITY_DEFINITIONS "-DAMSI_VERBOSE_1")
    elseif (ENABLE_VERBOSITY STREQUAL "MED")
        set(AMSI_VERBOSITY_DEFINITIONS "-DAMSI_VERBOSE_1 -DAMSI_VERBOSE_2")
    elseif (ENABLE_VERBOSITY STREQUAL "HIGH")
    set(AMSI_VERBOSITY_DEFINITIONS "-DAMSI_VERBOSE_1 -DAMSI_VERBOSE_2 -DAMSI_VERBOSE_3")
  elseif(ENABLE_VERBOSITY STREQUAL "OFF")
    set(AMSI_VERBOSITY_DEFINITIONS "")
  else()
    message(FATAL_ERROR "Something went wrong with VERBOSITY setting: ${ENABLE_VERBOSITY}\n Valid options are LOW,MED,HIGH,OFF")
  endif()
endif(ENABLE_VERBOSITY)
target_compile_definitions(util PUBLIC ${AMSI_VERBOSITY_DEFINITIONS})

install(TARGETS util
        EXPORT amsiutilTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

# this allows us to use the package from the
# build tree which is wanted for multiple build configs
export(EXPORT amsiutilTargets
       FILE "${CMAKE_BINARY_DIR}/amsi/amsiutilTargets.cmake"
       NAMESPACE amsi::)

install(EXPORT amsiutilTargets
        DESTINATION ${ConfigPackageLocation}
        NAMESPACE amsi::
        )

install(FILES ${util_public_headers} DESTINATION include/amsi)
# Headers need to be copied to binary directory to enable building against build
# W/O installation
file(COPY ${util_public_headers} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
