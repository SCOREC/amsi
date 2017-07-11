# Copyright (c) 2012 Marwan Abdellah <marwan.abdellah@epfl.ch>
#                    Daniel Nachbaur <daniel.nachbaur@epfl.ch>
#               2013 Stefan.Eilemann@epfl.ch

# Use pkg-config to fetch the contents of the .pc file
# After that, use the directories refer to the libraries and
# also the headers

find_package(PkgConfig QUIET)

if(HWLOC_LIB_DIR)
  set(ENV{PKG_CONFIG_PATH} "${HWLOC_LIB_DIR}/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif()

if(HWLOC_FIND_REQUIRED)
  set(_hwloc_OPTS "REQUIRED")
endif()
if(HWLOC_FIND_QUIETLY)
  set(_hwloc_OPTS "QUIET")
endif()
if(HWLOC_FIND_REQUIRED AND HWLOC_FIND_QUIETLY)
  set(_hwloc_OPTS "REQUIRED QUIET")
endif()

if(hwloc_FIND_VERSION)
  if(hwloc_FIND_VERSION_EXACT)
    pkg_check_modules(HWLOC ${_hwloc_OPTS} IMPORTED_TARGET hwloc=${hwloc_FIND_VERSION})
  else()
    pkg_check_modules(HWLOC ${_hwloc_OPTS} IMPORTED_TARGET hwloc>=${hwloc_FIND_VERSION})
  endif()
else()
  pkg_check_modules(HWLOC ${_hwloc_opts} IMPORTED_TARGET hwloc)
endif()

if(NOT HWLOC_FOUND)
  # check if hwloc_include_dir and hwloc_lib_dir are set
  find_path(HWLOC_INCLUDE_DIR hwloc.h
            HINTS ${HWLOC_INCLUDEDIR}
            PATH_SUFFIXES include)
  find_library(HWLOC_LIBRARIES hwloc
               HINTS ${HWLOC_LIB_DIR}
               PATH_SUFFIXES lib)
  if(HWLOC_INCLUDE_DIR AND HWLOC_LIBRARIES)
    set(HWLOC_FOUND TRUE)
  endif(HWLOC_INCLUDE_DIR AND HWLOC_LIBRARIES)
else(NOT HWLOC_FOUND)
  set(HWLOC_LIBRARIES "${HWLOC_STATIC_LDFLAGS};${HWLOC_STATIC_LIBRARIES}")
  set(HWLOC_INCLUDE_DIR ${HWLOC_INCLUDEDIR})
endif(NOT HWLOC_FOUND)

if(HWLOC_FOUND)
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(hwloc DEFAULT_MSG HWLOC_LIBRARIES HWLOC_INCLUDE_DIR)
  message(STATUS "Found hwloc ${HWLOC_VERSION} in ${HWLOC_INCLUDE_DIR}:${HWLOC_LIBRARIES}")
endif()
