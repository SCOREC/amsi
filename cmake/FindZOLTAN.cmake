# - Try to find zoltan
# Once done this will define
#  ZOLTAN_FOUND - System has ZOLTAN
#  ZOLTAN_INCLUDE_DIRS - The ZOLTAN include directories
#  ZOLTAN_LIBRARIES - The libraries needed to use ZOLTAN
#  ZOLTAN_DEFINITIONS - Compiler switches required for using ZOLTAN

if(NOT DEFINED PETSC_DIR)
  set(PETSC_DIR $ENV{PETSC_DIR})
endif()

if(NOT DEFINED PETSC_ARCH)
  set(PETSC_ARCH $ENV{PETSC_ARCH})
endif()

find_path(ZOLTAN_INCLUDE_DIR zoltan.h PATHS ${PETSC_DIR}/${PETSC_ARCH}/include)
find_library(ZOLTAN_LIBRARY libzoltan.a zoltan PATHS ${PETSC_DIR}/${PETSC_ARCH}/lib)

find_package(PARMETIS REQUIRED)
find_package(Scotch REQUIRED)

set(ZOLTAN_LIBRARIES ${ZOLTAN_LIBRARY} ${PARMETIS_LIBRARIES} ${SCOTCH_LIBRARIES})
set(ZOLTAN_INCLUDE_DIRS ${ZOLTAN_INCLUDE_DIR} ${PARMETIS_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ZOLTAN_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    ZOLTAN  
    DEFAULT_MSG
    ZOLTAN_LIBRARY ZOLTAN_INCLUDE_DIR
)

mark_as_advanced(ZOLTAN_INCLUDE_DIR ZOLTAN_LIBRARY)
