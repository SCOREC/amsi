cmake_minimum_required (VERSION 3.0)

project(amsi)

set(AMSI_MAJOR_VERSION 1)
set(AMSI_MINOR_VERSION 0)
set(AMSI_PATCH_VERSION 0)
set(AMSI_VERSION ${AMSI_MAJOR_VERSION}.${AMSI_MINOR_VERSION}.${AMSI_PATCH_VERSION})

# add FindPACKAGE.cmake files to path
list(APPEND CMAKE_MODULE_PATH $ENV{DEVROOT}/scripts/cmake ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_DEBUG_POSTFIX d)

set(HOSTNAME $ENV{HOSTNAME})
if("$HOSTNAME" STREQUAL "")
  set(HOSTNAME $ENV{HOST})
endif("$HOSTNAME" STREQUAL "")
message(STATUS "Configuring to build on" ${HOSTNAME})

if("${HOSTNAME}" STREQUAL "q.ccni.rpi.edu")
  set(HOST BGQ)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
  SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
  SET_PROPERTY(GLOBAL PROPERTY TARGET_ARCHIVES_MAY_BE_SHARED_LIBS FALSE)
  #-D__GXX_EXPERIMENTAL_CXX0X__
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qlanglvl=extended")
  set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
  set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
  set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
  set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
else()
  set(HOST SCOREC)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .so .a)
  list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -Wall")
endif()

include(GenerateExportHeader)
set(ConfigPackageLocation lib/cmake/amsi)

message(STATUS "Configuring amsi_util")
add_subdirectory(util)

message(STATUS "Configuring amsi_meta")
add_subdirectory(meta)

message(STATUS "Configuring amsi_interface")
add_subdirectory(interface)

message(STATUS "Configuring amsi_control")
add_subdirectory(control)

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
                    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating API documentation with doxygen." VERBATIM)
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

