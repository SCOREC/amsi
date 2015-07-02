# - Try to find apf
# Once done this will define
#  APF_FOUND - System has APF
#  APF_INCLUDE_DIRS - The APF include directories
#  APF_LIBRARIES - The libraries needed to use APF
#  APF_DEFINITIONS - Compiler switches required for using APF


find_path(APF_INCLUDE_DIR apf.h)

find_library(APF_LIBRARY apf)

find_package(PCU REQUIRED)

set(APF_LIBRARIES ${APF_LIBRARY} )
set(APF_INCLUDE_DIRS ${APF_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set APF_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    APF  
    DEFAULT_MSG
    APF_LIBRARY APF_INCLUDE_DIR
)

mark_as_advanced(APF_INCLUDE_DIR APF_LIBRARY )
