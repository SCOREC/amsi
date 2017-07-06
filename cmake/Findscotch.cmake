include(util)

#check for explicit parameters
checkSetParam(PETSC_DIR FALSE)
checkSetParam(PETSC_ARCH FALSE)
checkSetParam(SCOTCH_DIR FALSE)

#check for pkg?

find_library(PTSCOTCH_LIBRARY ptscotch
             HINTS ${PETSC_DIR}/${PETSC_ARCH} ${SCOTCH_DIR} 
             PATH_SUFFIXES lib)

find_library(PTSCOTCHERR_LIBRARY ptscotcherr 
             HINTS ${PETSC_DIR}/${PETSC_ARCH} ${SCOTCH_DIR} 
             PATH_SUFFIXES lib)

find_library(SCOTCH_LIBRARY scotch 
             HINTS ${PETSC_DIR}/${PETSC_ARCH} ${SCOTCH_DIR}
             PATH_SUFFIXES lib)

find_library(SCOTCHERR_LIBRARY scotcherr
             HINTS ${PETSC_DIR}/${PETSC_ARCH} ${SCOTCH_DIR}
             PATH_SUFFIXES lib)

set(SCOTCH_LIBRARIES ${PTSCOTCH_LIBRARY} 
                     ${PTSCTOCHERR_LIBRARY} 
                     ${SCOTCH_LIBRARY} 
                     ${SCOTCHERR_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SCOTCH DEFAULT_MSG SCOTCH_LIBRARIES)
mark_as_advanced(SCOTCH_LIBRARIES)
