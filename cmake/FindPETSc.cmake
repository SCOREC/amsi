set(PETSC_DIR $ENV{PETSC_DIR})
set(PETSC_ARCH $ENV{PETSC_ARCH})

if( "${PETSC_DIR}" STREQUAL "" )
  message(SEND_ERROR "Please specify the PETSC_DIR environment variable.")
endif()

if( "${PETSC_ARCH}" STREQUAL "" )
  message(SEND_ERROR "Please specify the PETSC_ARCH environment variable.")
endif()

find_package(PkgConfig)
set(ENV{PKG_CONFIG_PATH} "${PETSC_DIR}/${PETSC_ARCH}/lib/pkgconfig/")

pkg_check_modules(PC_PETSC PETSc)

find_path(PETSC_INCLUDE_DIR_ONE petscksp.h
          HINTS ${PC_PETSC_INCLUDEDIR}
          PATH_SUFFIXES include)

find_path(PETSC_INCLUDE_DIR_TWO petscfix.h
          HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
          PATH_SUFFIXES include)

set(PETSC_INCLUDE_DIRS ${PETSC_INCLUDE_DIR_ONE} ${PETSC_INCLUDE_DIR_TWO})

find_library(PETSC_LIB petsc
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(SUPERLU_LIB superlu_dist_3.3
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(PARMETIS_LIB parmetis
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(METIS_LIB metis
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(F2CLAPACK_LIB f2clapack
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(F2CBLAS_LIB f2cblas
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

set(PETSC_LIBRARIES ${PETSC_LIB} ${SUPERLU_LIB} ${PARMETIS_LIB} ${METIS_LIB} ${F2CLAPACK_LIB} ${F2CBLAS_LIB} -lX11 -lssl)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PETSc DEFAULT_MSG PETSC_LIBRARIES PETSC_INCLUDE_DIRS)
mark_as_advanced(PETSC_LIBRARIES PETSC_INCLUDE_DIRS)


