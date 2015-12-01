# - Try to find zoltan
# Once done this will define
#  SCOTCH_FOUND - System has SCOTCH
#  SCOTCH_INCLUDE_DIRS - The SCOTCH include directories
#  SCOTCH_LIBRARIES - The libraries needed to use SCOTCH
#  SCOTCH_DEFINITIONS - Compiler switches required for using SCOTCH

if(NOT DEFINED PETSC_DIR)
  set(PETSC_DIR $ENV{PETSC_DIR})
endif()

if(NOT DEFINED PETSC_ARCH)
  set(PETSC_ARCH $ENV{PETSC_ARCH})
endif()

find_library(PTSCOTCH_LIBRARY    ptscotch    HINTS ${PETSC_DIR}/${PETSC_ARCH} PATH_SUFFIXES lib)
find_library(PTSCOTCHERR_LIBRARY ptscotcherr HINTS ${PETSC_DIR}/${PETSC_ARCH} PATH_SUFFIXES lib)
find_library(SCOTCH_LIBRARY      scotch      HINTS ${PETSC_DIR}/${PETSC_ARCH} PATH_SUFFIXES lib)
find_library(SCOTCHERR_LIBRARY   scotcherr   HINTS ${PETSC_DIR}/${PETSC_ARCH} PATH_SUFFIXES lib)

set(SCOTCH_LIBRARIES ${PTSCOTCH_LIBRARY} 
                     ${PTSCTOCHERR_LIBRARY} 
                     ${SCOTCH_LIBRARY} 
                     ${SCOTCHERR_LIBRARY})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SCOTCH_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    SCOTCH  
    DEFAULT_MSG
    SCOTCH_LIBRARIES
)

mark_as_advanced(SCOTCH_LIBRARIES)
