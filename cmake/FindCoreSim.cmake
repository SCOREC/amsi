# CORE_INCLUDE_DIRS
# CORE_LIBRARIES

find_package(SimModSuite REQUIRED)

find_path(PCU_INCLUDE_DIR PCU.h)
find_path(APF_INCLUDE_DIR apf.h)
find_path(APF_SIM_INCLUDE_DIR apfSIM.h)
find_path(GMI_INCLUDE_DIR gmi.h)
find_path(GMI_SIM_INCLUDE_DIR gmi_sim.h)
find_path(SPR_INCLUDE_DIR spr.h)
find_path(MA_INCLUDE_DIR ma.h)

set(CORE_INCLUDE_DIRS ${PCU_INCLUDE_DIR} 
			 ${APF_INCLUDE_DIR} 
			 ${APF_SIM_INCLUDE_DIR} 
			 ${GMI_INCLUDE_DIR} 
			 ${GMI_SIM_INCLUDE_DIR}
			 ${SPR_INCLUDE_DIR}
			 ${MA_INCLUDE_DIR}
	                 ${SIMMODSUITE_INCLUDE_DIRS})

find_library(PCU_LIB pcu)
find_library(APF_LIB apf)
find_library(APF_SIM_LIB apf_sim)
find_library(GMI_LIB gmi)
find_library(GMI_SIM_LIB gmi_sim)
find_library(SPR_LIB spr)
find_library(MA_LIB ma)

set(CORE_LIBRARIES ${MA_LIB} ${APF_SIM_LIB} ${APF_LIB} ${GMI_SIM_LIB} ${GMI_LIB} ${SPR_LIB} ${PCU_LIB} ${SIMMODSUITE_LIBS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CORE DEFAULT_MSG CORE_LIBRARIES CORE_INCLUDE_DIRS)

mark_as_advanced(CORE_LIBRARIES CORE_INCLUDE_DIRS)

