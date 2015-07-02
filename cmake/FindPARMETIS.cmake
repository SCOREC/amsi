# - Try to find zoltan
# Once done this will define
#  PARMETIS_FOUND - System has PARMETIS
#  PARMETIS_INCLUDE_DIRS - The PARMETIS include directories
#  PARMETIS_LIBRARIES - The libraries needed to use PARMETIS
#  PARMETIS_DEFINITIONS - Compiler switches required for using PARMETIS

find_path(PARMETIS_INCLUDE_DIR parmetis.h PATHS $ENV{PETSC_DIR}/$ENV{PETSC_ARCH}/include)
find_path(METIS_INCLUDE_DIR metis.h PATHS $ENV{PETSC_DIR}/$ENV{PETSC_ARCH}/include)

find_library(PARMETIS_LIBRARY libparmetis.a parmetis PATHS $ENV{PETSC_DIR}/$ENV{PETSC_ARCH}/lib)
find_library(METIS_LIBRARY libmetis.a metis PATHS $ENV{PETSC_DIR}/$ENV{PETSC_ARCH}/lib)

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
