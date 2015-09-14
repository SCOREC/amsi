cmake_minimum_required (VERSION 3.0)
project(amsi)
set(AMSI_MAJOR_VERSION 1)
set(AMSI_MINOR_VERSION 0)
set(AMSI_PATCH_VERSION 0)
set(AMSI_VERSION ${AMSI_MAJOR_VERSION}.${AMSI_MINOR_VERSION}.${AMSI_PATCH_VERSION})

# add FindPACKAGE.cmake files to path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
# the toolchain file on the Q also specifies this, but we still find shared if we don't specify it again...
if($ENV{HOST} STREQUAL "q.ccni.rpi.edu")
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_definitions(-DDEBUG)
endif()

# TODO: determine which compiler family is in use and change the C++11 flag appropriately -qlanglvl= or something
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(HOST SCOREC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "XL")
  set(HOST BGQ)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qlanglvl=extended")
endif()

include(GenerateExportHeader)
set(ConfigPackageLocation lib/cmake/amsi)

find_package(CoreSim REQUIRED)

add_subdirectory(amsi_util)

add_subdirectory(amsi_meta)

add_subdirectory(amsi_interface)

add_subdirectory(amsi_control)

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/amsi/amsiConfigVersion.cmake"
  VERSION ${AMSI_VERSION}
  COMPATIBILITY AnyNewerVersion )

configure_file(cmake/amsiConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/amsi/amsiConfig.cmake"
  COPYONLY)

install(FILES cmake/amsiConfig.cmake "${CMAKE_CURRENT_BINARY_DIR}/amsi/amsiConfigVersion.cmake"
  DESTINATION ${ConfigPackageLocation}
  COMPONENT Devel)

