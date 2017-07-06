include(util)

# check for explicit parameters
checkSetParam(PETSC_DIR FALSE)
checkSetParam(PETSC_ARCH FALSE)
checkSetParam(ZOLTAN_DIR FALSE)

# check for pkg?

find_path(ZOLTAN_INCLUDE_DIR zoltan.h 
          HINTS ${PETSC_DIR}/${PETSC_ARCH} ${ZOLTAN_DIR}
          PATH_SUFFIXES include)

find_library(ZOLTAN_LIBRARY libzoltan.a zoltan 
             HINTS ${PETSC_DIR}/${PETSC_ARCH} ${ZOLTAN_DIR}
             PATH_SUFFIXES lib)

find_package(parmetis REQUIRED)
# todo (m) Bill : only check for scotch if zoltan is a petsc external package, since otherwise it is not a required dependency of zoltan
find_package(scotch REQUIRED)

set(ZOLTAN_LIBRARIES ${ZOLTAN_LIBRARY} ${PARMETIS_LIBRARIES} ${SCOTCH_LIBRARIES})
set(ZOLTAN_INCLUDE_DIRS ${ZOLTAN_INCLUDE_DIR} ${PARMETIS_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZOLTAN DEFAULT_MSG
  ZOLTAN_LIBRARIES 
  ZOLTAN_INCLUDE_DIRS)
mark_as_advanced(ZOLTAN_INCLUDE_DIRS ZOLTAN_LIBRARIES)
