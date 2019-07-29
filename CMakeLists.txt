cmake_minimum_required(VERSION 3.0)

project(amsi)

set(AMSI_MAJOR_VERSION 1)
set(AMSI_MINOR_VERSION 0)
set(AMSI_PATCH_VERSION 2)
set(AMSI_VERSION ${AMSI_MAJOR_VERSION}.${AMSI_MINOR_VERSION}.${AMSI_PATCH_VERSION})

# add FindPACKAGE.cmake files to path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/util.cmake)
set(CMAKE_DEBUG_POSTFIX d)
site_name(HOSTNAME)
message(STATUS "Configuring to build on " ${HOSTNAME})

set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
if("${HOSTNAME}" STREQUAL "q.ccni.rpi.edu")
  set(HOST BGQ)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qlanglvl=extended -qlanglvl=extended0x")
  message("HOST SET to BGQ")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "XL")
  set(HOST SCOREC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  message("HOST SET to SCOREC/Generic")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(HOST SCOREC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wno-unused-parameter -Wextra -Werror")
  message("HOST SET to SCOREC/Generic")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(HOST SCOREC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Werror")
  message("HOST SET to SCOREC/Generic")
endif()
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
SET_PROPERTY(GLOBAL PROPERTY TARGET_ARCHIVES_MAY_BE_SHARED_LIBS FALSE)
set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

option(BUILD_TESTS "Build amsi tests" ON)
if(BUILD_TESTS)
  include(CTest)
  #SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
  #SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
  endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
endif(BUILD_TESTS)

include(GenerateExportHeader)
set(ConfigPackageLocation lib/cmake/amsi)

find_package(SCOREC CONFIG REQUIRED)

message(STATUS "Configuring util")
add_subdirectory(util)

message(STATUS "Configuring multiscale")
add_subdirectory(multiscale)

message(STATUS "Configuring analysis")
add_subdirectory(analysis)

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
