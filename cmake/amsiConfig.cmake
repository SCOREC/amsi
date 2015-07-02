include(CMakeFindDependencyMacro)
#find_dependency(MPI)

#include("${CMAKE_CURRENT_LIST_DIR}/amsiTargets.cmake}")

set(_supported_components util meta interface control)
set(amsi_FOUND TRUE)
foreach(_comp ${amsi_FIND_COMPONENTS})
  if(NOT "${_supported_components}" MATCHES "${_comp}")
    set(amsi_FOUND FALSE)
    set(amsi_NOTFOUND_MESSAGE "Unsupported component: ${_comp}")
  endif()
  include("${CMAKE_CURRENT_LIST_DIR}/amsi${_comp}Targets.cmake")
endforeach()
