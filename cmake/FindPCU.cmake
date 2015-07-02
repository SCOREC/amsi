# - Try to find zoltan
# Once done this will define
#  PCU_FOUND - System has PCU
#  PCU_INCLUDE_DIRS - The PCU include directories
#  PCU_LIBRARIES - The libraries needed to use PCU
#  PCU_DEFINITIONS - Compiler switches required for using PCU


find_path(PCU_INCLUDE_DIR PCU.h)

find_library(PCU_LIBRARY pcu)

set(PCU_LIBRARIES ${PCU_LIBRARY} )
set(PCU_INCLUDE_DIRS ${PCU_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PCU_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    PCU  
    DEFAULT_MSG
    PCU_LIBRARY PCU_INCLUDE_DIR
)

mark_as_advanced(PCU_INCLUDE_DIR PCU_LIBRARY )
