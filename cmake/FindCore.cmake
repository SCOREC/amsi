# CORE_INCLUDE_DIRS
# CORE_LIBRARIES

find_path(PCU_INCLUDE_DIR PCU.h)
find_path(APF_INCLUDE_DIR apf.h)
find_path(GMI_INCLUDE_DIR gmi.h)
find_path(SPR_INCLUDE_DIR spr.h)

set(CORESIM_INCLUDE_DIRS ${PCU_INCLUDE_DIR} 
			 ${APF_INCLUDE_DIR} 
			 ${GMI_INCLUDE_DIR} 
			 ${SPR_INCLUDE_DIR})

find_library(PCU_LIB pcu)
find_library(APF_LIB apf)
find_library(GMI_LIB gmi)
find_library(SPR_LIB spr)

set(CORE_LIBRARIES ${APF_LIB} ${GMI_LIB} ${SPR_LIB} ${PCU_LIB})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CORE DEFAULT_MSG CORE_LIBRARIES CORE_INCLUDE_DIRS)

mark_as_advanced(CORE_LIBRARIES CORE_INCLUDE_DIRS)

