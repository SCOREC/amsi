cmake_minimum_required (VERSION 3.0)
project(amsi)
set(AMSI_MAJOR_VERSION 1)
set(AMSI_MINOR_VERSION 0)
set(AMSI_PATCH_VERSION 0)
set(AMSI_VERSION ${AMSI_MAJOR_VERSION}.${AMSI_MINOR_VERSION}.${AMSI_PATCH_VERSION})

include(CTest)

# add FindPACKAGE.cmake files to path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_definitions(-DDEBUG)
endif()

# TODO: determine which compiler family is in use and change the C++11 flag appropriately -qlanglvl= or something
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(HOST SCOREC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  set(HOST BGQ)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qlanglvl=extended")
  include_directories(${BOOST_INCLUDE_DIR})
endif()

find_package(MPI REQUIRED)

find_package(PETSc)
if(PETSC_FOUND)
  set(HAVE_PETSC PETSC)
  include_directories(${PETSC_INCLUDE_DIRS})
  message(STATUS "PETSc libraries: ${PETSC_LIBRARIES}")
else()
  set(HAVE_PETSC NO_PETSC)
endif()

find_package(ZOLTAN)

if(ZOLTAN_FOUND)
  set(HAVE_ZOLTAN ZOLTAN)
  include_directories(${ZOLTAN_INCLUDE_DIRS})
else()
  set(HAVE_ZOLTAN NO_ZOLTAN)
endif()

# attempt to find PCU optional dependency
find_package(CoreSim)
if(CORESIM_FOUND)
  set(CORE_FOUND TRUE)
  set(HAVE_CORE CORE)
  set(HAVE_MESHSIM SIM)
else()
  set(HAVE_CORE NO_CORE)
  set(HAVE_MESHSIM NO_SIM)
endif()

include(GenerateExportHeader)
set(ConfigPackageLocation lib/cmake/amsi)

add_subdirectory(amsi_util)

add_subdirectory(amsi_meta)

add_subdirectory(amsi_interface)

add_subdirectory(amsi_control)

add_subdirectory(test)

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

