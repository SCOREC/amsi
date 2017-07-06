include(util)

# check for explicit parameters
checkSetParam(PETSC_DIR FALSE)
checkSetParam(PETSC_ARCH FALSE)
checkSetParam(PARMETIS_DIR FALSE)

# check for pkg?

find_path(PARMETIS_INCLUDE_DIR parmetis.h 
          HINTS ${PETSC_DIR}/${PETSC_ARCH} ${PARMETIS_DIR}
	  PATH_SUFFIXES include)
	
find_path(METIS_INCLUDE_DIR metis.h 
          HINTS ${PETSC_DIR}/${PETSC_ARCH} ${PARMETIS_DIR}
          PATH_SUFFIXES include)

find_library(PARMETIS_LIBRARY libparmetis.a parmetis 
             HINTS ${PETSC_DIR}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

find_library(METIS_LIBRARY libmetis.a metis 
             HINTS ${PETSC_DIR}/${PETSC_ARCH}
             PATH_SUFFIXES lib)

set(PARMETIS_LIBRARIES ${PARMETIS_LIBRARY} ${METIS_LIBRARY})
set(PARMETIS_INCLUDE_DIRS ${PARMETIS_INCLUDE_DIR} ${METIS_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PARMETIS DEFAULT_MSG
  PARMETIS_LIBRARIES 
  PARMETIS_INCLUDE_DIRS)
mark_as_advanced(PARMETIS_INCLUDE_DIRS PARMETIS_LIBRARIES)
