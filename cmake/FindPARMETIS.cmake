# - Try to find zoltan
# Once done this will define
#  PARMETIS_FOUND - System has PARMETIS
#  PARMETIS_INCLUDE_DIRS - The PARMETIS include directories
#  PARMETIS_LIBRARIES - The libraries needed to use PARMETIS
#  PARMETIS_DEFINITIONS - Compiler switches required for using PARMETIS

if(NOT DEFINED PETSC_DIR)
  set(PETSC_DIR $ENV{PETSC_DIR})
endif()

if(NOT DEFINED PETSC_ARCH)
  set(PETSC_ARCH $ENV{PETSC_ARCH})
endif()

find_path(PARMETIS_INCLUDE_DIR parmetis.h PATHS ${PETSC_DIR}/${PETSC_ARCH}/include)
find_path(METIS_INCLUDE_DIR metis.h PATHS ${PETSC_DIR}/${PETSC_ARCH}/include)

find_library(PARMETIS_LIBRARY libparmetis.a parmetis PATHS ${PETSC_DIR}/${PETSC_ARCH}/lib)
find_library(METIS_LIBRARY libmetis.a metis PATHS ${PETSC_DIR}/${PETSC_ARCH}/lib)

set(PARMETIS_LIBRARIES ${PARMETIS_LIBRARY} ${METIS_LIBRARY})
set(PARMETIS_INCLUDE_DIRS ${PARMETIS_INCLUDE_DIR} ${METIS_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PARMETIS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    PARMETIS  
    DEFAULT_MSG
    PARMETIS_LIBRARIES PARMETIS_INCLUDE_DIRS
)

mark_as_advanced(PARMETIS_INCLUDE_DIRS PARMETIS_LIBRARIES)
