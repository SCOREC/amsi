include(util)
if(NOT PETSC_FOUND)
  checkSetParam(PETSC_DIR TRUE)
  checkSetParam(PETSC_ARCH TRUE)

  find_path(PETSC_CONFIG_DIR PETScBuildInternal.cmake
    HINTS ${PETSC_DIR}/${PETSC_ARCH}/lib/petsc/conf/)

  include(${PETSC_CONFIG_DIR}/PETScBuildInternal.cmake)

  find_path(PETSC_INCLUDE_DIR_ONE petscksp.h
    HINTS ${PETSC_DIR}
    PATH_SUFFIXES include)

  find_path(PETSC_INCLUDE_DIR_TWO petscfix.h
    HINTS ${PETSC_DIR}/${PETSC_ARCH}
    PATH_SUFFIXES include)

  set(PETSC_INCLUDE_DIRS ${PETSC_PACKAGE_INCLUDES} ${PETSC_INCLUDE_DIR_ONE} ${PETSC_INCLUDE_DIR_TWO})
  #describeVar(PETSC_INCLUDE_DIRS)

  find_library(PETSC_LIB petsc
    HINTS ${PETSC_DIR}/${PETSC_ARCH}
    PATH_SUFFIXES lib)

  set(PETSC_LIBRARIES ${PETSC_LIB} ${PETSC_PACKAGE_LIBS})
  #describeVar(PETSC_LIBRARIES)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(PETSc DEFAULT_MSG
    PETSC_LIBRARIES
    PETSC_INCLUDE_DIRS)
  mark_as_advanced(PETSC_LIBRARIES PETSC_INCLUDE_DIRS)
endif(NOT PETSC_FOUND)

