# CORE_INCLUDE_DIRS
# CORE_LIBS

# TODO (M) Bill : move this into a common cmake util file
macro(prependList lst prfx nwlst)
  foreach(itm ${lst})
    list(APPEND ${nwlst} ${prfx}${itm})
  endforeach()
endmacro()

macro(pkgLibCheck pkgnm prefix libs isRequired)
  foreach(lib ${libs})
    string(TOUPPER "${lib}" upperlib)
    message(STATUS ${prefix}_${upperlib}_LIBRARY_DIRS)
    message(STATUS ${${prefix}_${upperlib}_LIBRARY_DIRS})

    set(lib_path ${${prefix}_${upperlib}_LIBRARY_DIRS})
    unset(find_lib CACHE)
    find_library(find_lib "${lib}" PATHS ${lib_path})
    if(NOT ${lib}_FOUND)
      if(${isRequired})
        message(FATAL_ERROR "library ${lib} not found in ${lib_path}")
      else()
        message(STATUS "library ${lib} not found in ${lib_path}")
      endif()
    else()
      set("${pkgnm}_${lib}_FOUND" TRUE CACHE INTERNAL "Library present")
      set(${pkgnm}_LIBS ${pkgnm}_LIBS ${find_lib} ${${prefix}_${lib}_LIBRARIES})
    endif()
  endforeach()
endmacro(pkgLibCheck)

if(NOT EXISTS "${CORE_INSTALL_DIR}")
  if(NOT EXISTS "$ENV{CORE_INSTALL_DIR}")
    message(FATAL_ERROR "Please specify CORE_INSTALL_DIR")
  else()
    set(CORE_INSTALL_DIR $ENV{CORE_INSTALL_DIR})
  endif()
endif()

find_package(PkgConfig)
set(EVN{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CORE_INSTALL_DIR}/lib/pkgconfig/")

find_path(CORE_INCLUDE_DIRS
  NAMES apf.h gmi.h ma.h mth.h PCU.h
  PATHS ${CORE_INSTALL_DIR}
  PATH_SUFFIXES include)

# set the libraries to look for depending on whether simmetrix is required/present
set(LIBS ma mds gmi)
message(STATUS "Looking for ${CORESIM_FIND_COMPONENTS}")
if(";${CORE_FIND_COMPONENTS};" MATCHES ";Sim;")
  if(CORE_FIND_REQUIRED_SIM)
    find_package(SimModSuite REQUIRED)
  else()
    find_package(SimModSuite)
  endif()
  if(SIMMODSUITE_FOUND)
    set(LIBS ${LIBS} gmi_sim apf apf_sim)
  else()
    set(LIBS ${LIBS} apf)
  endif()
else()
    set(LIBS ${LIBS} apf)
endif()
set(LIBS ${LIBS} spr parma ph dsp lion)

set(prefix PC_CORE)
set(pkglibs "")
prependList("${LIBS}" lib pkglibs)
pkg_check_modules(${prefix} ${pkglibs})
set(static_prefix ${prefix}_STATIC)
pkgLibCheck(CORE ${static_prefix} "${LIBS}" TRUE)

if(SIMMODSUITE_FOUND)
  set(CORE_LIBS ${CORE_LIBS} ${SIMMETRIX_LIBS})
  set(CORE_INCLUDE_DIRS ${CORE_INCLUDE_DIRS} ${SIMMETRIX_LIBS})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CORE DEFAULT_MSG CORE_LIBS CORE_INCLUDE_DIRS)
mark_as_advanced(CORE_LIBS CORE_INCLUDE_DIRS)

