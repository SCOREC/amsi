cmake_minimum_required(VERSION 3.21.1)
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

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build amsi tests" ON)
if (BUILD_TESTS)
    include(CTest)
    #SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
    #SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
endif (BUILD_TESTS)

include(GenerateExportHeader)
set(ConfigPackageLocation lib/cmake/amsi)

find_package(SCOREC CONFIG REQUIRED)

option(BUILD_EXTERNAL "build external dependencies" ON)
if (BUILD_EXTERNAL)
    add_subdirectory(external)
endif ()


message(STATUS "Configuring util")
add_subdirectory(util)

message(STATUS "Configuring multiscale")
add_subdirectory(multiscale)

message(STATUS "Configuring analysis")
add_subdirectory(analysis)

find_package(MPI REQUIRED)
# top level amsi library. Eventually this should get reorganized into a src/ folder
add_library(amsi_amsi amsi.cc)
target_link_libraries(amsi_amsi PUBLIC amsi::util amsi::multiscale amsi::analysis MPI::MPI_CXX)
add_library(amsi::amsi ALIAS amsi_amsi)
SET(AMSI_PUBLIC_HEADERS amsi.h util/amsiConfigurationOptions.h)
set_target_properties(amsi_amsi PROPERTIES EXPORT_NAME amsi PUBLIC_HEADER "${AMSI_PUBLIC_HEADERS}")
target_include_directories(amsi_amsi PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include/amsi>
        )
install(TARGETS amsi_amsi
        EXPORT amsi_amsi
        DESTINATION lib
        PUBLIC_HEADER DESTINATION include/amsi
        )
export(EXPORT amsi_amsi
        NAMESPACE amsi::
        FILE amsiamsiTargets.cmake
        )
install(EXPORT amsi_amsi
        DESTINATION lib/cmake/amsi
        NAMESPACE amsi::
        FILE amsiamsiTargets.cmake
        )

if (BUILD_TESTS)
    add_subdirectory(test)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake @ONLY)
endif ()

find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(amsi-doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with doxygen." VERBATIM)
endif ()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/amsi/amsiConfigVersion.cmake"
        VERSION ${AMSI_VERSION}
        COMPATIBILITY AnyNewerVersion)

configure_file(cmake/amsiConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/amsi/amsiConfig.cmake"
        COPYONLY)

install(FILES cmake/amsiConfig.cmake "${CMAKE_CURRENT_BINARY_DIR}/amsi/amsiConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation}
        COMPONENT Devel)
