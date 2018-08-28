# - Try to find parmetis
# Once done this will define
#  PTSCOTCH_FOUND - System has PTSCOTCH
#  PTSCOTCH_INCLUDE_DIRS - The PTSCOTCH include directories
#  PTSCOTCH_LIBRARIES - The libraries needed to use PTSCOTCH
#  PTSCOTCH_DEFINITIONS - Compiler switches required for using PTSCOTCH

set(PTSCOTCH_PREFIX "" CACHE STRING "PTSCOTCH install directory")

find_path(PTSCOTCH_INCLUDE_DIR scotch.h PATHS "${PTSCOTCH_PREFIX}/include")
if(NOT EXISTS "${PTSCOTCH_INCLUDE_DIR}")
  message(FATAL_ERROR "PTSCOTCH include dir not found")
endif()

  
find_library(PTSCOTCH_LIBRARY ptscotch ptscotcherr ptscotcherrexit ptscotchparmetis PATHS "${PTSCOTCH_PREFIX}/lib")
if(NOT EXISTS "${PTSCOTCH_LIBRARY}")
  message(FATAL_ERROR "PTSCOTCH library not found")
endif()

find_library(SCOTCH_LIBRARY scotch scotch scotcherrexit  PATHS "${PTSCOTCH_PREFIX}/lib")
if(NOT EXISTS "${SCOTCH_LIBRARY}")
  message(FATAL_ERROR "scotch library not found")
endif()


set(PTSCOTCH_LIBRARIES ${PTSCOTCH_LIBRARY} ${SCOTCH_LIBRARY})
set(PTSCOTCH_INCLUDE_DIRS ${PTSCOTCH_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PTSCOTCH_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(PTSCOTCH  DEFAULT_MSG
    PTSCOTCH_LIBRARY SCOTCH_LIBRARY PTSCOTCH_INCLUDE_DIR)

mark_as_advanced(PTSCOTCH_INCLUDE_DIR PTSCOTCH_LIBRARY SCOTCH_LIBRARY)
