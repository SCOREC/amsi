set(PETSC_DIR $ENV{PETSC_DIR})
set(PETSC_ARCH $ENV{PETSC_ARCH})

if( "${PETSC_DIR}" STREQUAL "" )
  message(SEND_ERROR "Please specify the PETSC_DIR environment variable.")
endif()

if( "${PETSC_ARCH}" STREQUAL "" )
  message(WARNING "PETSC_ARCH not specified.")
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

find_library(SUPERLU_LIB superlu_dist_4.1
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_package(PARMETIS)
find_package(ZOLTAN)

find_library(PTSCOTCH_LIB ptscotch
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(PTSCOTCHERR_LIB ptscotcherr
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(SCOTCH_LIB scotch
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(SCOTCHERR_LIB scotcherr
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)
	   
set(SCOTCH_LIBS ${PTSCOTCH_LIB} ${PTSCOTCHERR_LIB} ${SCOTCH_LIB} ${SCOTCHERR_LIB})

find_library(LAPACK_LIB f2clapack
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

if(NOT ${LAPACK_LIB})
  find_library(LAPACK_LIB flapack
               HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
               PATH_SUFFIXES lib)
endif()

find_library(BLAS_LIB f2cblas
             HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

if(NOT {BLAS_LIB})
  find_library(BLAS_LIB fblas
               HINTS ${PC_PETSC_PREFIX}/${PETSC_ARCH}
               PATH_SUFFIXES lib)
endif()


set(PETSC_LIBRARIES ${PETSC_LIB} 
                    ${SUPERLU_LIB} 
                    ${PARMETIS_LIBRARIES} 
                    ${ZOLTAN_LIBRARIES}
		    ${SCOTCH_LIBS})
if(LAPACK_LIB)
  list(APPEND PETSC_LIBRARIES ${LAPACK_LIB})
endif()
if(BLAS_LIB)
  list(APPEND PETSC_LIBRARIES ${BLAS_LIB})
endif()
list(APPEND PETSC_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PETSc DEFAULT_MSG PETSC_LIBRARIES PETSC_INCLUDE_DIRS)
mark_as_advanced(PETSC_LIBRARIES PETSC_INCLUDE_DIRS)


