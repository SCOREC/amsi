include(CMakeFindDependencyMacro)

find_dependency(ModelTraits)
find_dependency(MPI)
find_dependency(PkgConfig ${extraArgs})
pkg_check_modules(PETSc ${extraArgs} IMPORTED_TARGET PETSc)

include(${CMAKE_CURRENT_LIST_DIR}/amsiutilTargets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/amsimultiscaleTargets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/amsianalysisTargets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/amsiamsiTargets.cmake)
