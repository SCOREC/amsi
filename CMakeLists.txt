cmake_minimum_required (VERSION 3.0)

project(amsi)

set(AMSI_MAJOR_VERSION 1)
set(AMSI_MINOR_VERSION 0)
set(AMSI_PATCH_VERSION 0)
set(AMSI_VERSION ${AMSI_MAJOR_VERSION}.${AMSI_MINOR_VERSION}.${AMSI_PATCH_VERSION})

# add FindPACKAGE.cmake files to path
list(APPEND CMAKE_MODULE_PATH $ENV{DEVROOT}/scripts/cmake ${CMAKE_CURRENT_LIST_DIR}/cmake)
include($ENV{DEVROOT}/scripts/cmake/util.cmake)
set(CMAKE_DEBUG_POSTFIX d)
site_name(HOSTNAME)
message(STATUS "Configuring to build on " ${HOSTNAME})

if("${HOSTNAME}" STREQUAL "q.ccni.rpi.edu")
  set(HOST BGQ)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
  SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
  SET_PROPERTY(GLOBAL PROPERTY TARGET_ARCHIVES_MAY_BE_SHARED_LIBS FALSE)
  #-D__GXX_EXPERIMENTAL_CXX0X__
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qlanglvl=extended -qlanglvl=extended0x")
  set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
  set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
  set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
  set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
else()
  set(HOST SCOREC)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")
  # list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -Wall")
endif()

if(BUILD_TESTS)
  SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
  SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
  endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
endif(BUILD_TESTS)

include(GenerateExportHeader)
set(ConfigPackageLocation lib/cmake/amsi)

find_package(SCOREC CONFIG REQUIRED)

message(STATUS "Configuring util")
add_subdirectory(util)

message(STATUS "Configuring meta")
add_subdirectory(meta)

message(STATUS "Configuring interface")
add_subdirectory(interface)

message(STATUS "Configuring control")
add_subdirectory(control)

if(BUILD_TESTS)
  add_subdirectory(test)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake.in
                 ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake @ONLY)
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

