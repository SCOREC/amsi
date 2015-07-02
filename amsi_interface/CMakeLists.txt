find_package(CoreSim)
find_package(PETSc)

set(interface_public_headers
    amsiInterface.h
    ElementalSystem.h
    MatrixUtil.h
    FullLULAS.h
    LAS.h
    FEA.h
    Solvers.h
    TensorFieldQuery.h
    NonFiniteElement.h
    ConvenienceFunctions.h
   )

set(interface_src_files
    amsiInterface.cc
    MatrixUtil.h
    FullLULAS.cc
    FEA.cc
    Solvers.cc
    NonFiniteElement.cc
    ConvenienceFunctions.cc
   )

if(PETSC_FOUND)
  list(APPEND interface_public_headers PetscLAS.h)
  list(APPEND interface_src_files PetscLAS.cc)
  include_directories(${PETSC_INCLUDES})
endif(PETSC_FOUND)

if(CORE_FOUND)
  list(APPEND interface_public_headers
       ElementalSystem.h
       NeumannIntegrator.h
       apfFunctions.h
       apfFEA.h
       LinearElasticIntegrator.h
       StressStrainIntegrator.h
       SurfaceTractionIntegrator.h
       NonLinElasticIntegrator.h
       NonLinearElasticIntegrator.h
       TrussIntegrator.h)

  list(APPEND interface_src_files
       ElementalSystem.cc
       NeumannIntegrator.cc
       apfFunctions.cc
       apfFEA.cc
       LinearElasticIntegrator.cc
       StressStrainIntegrator.cc
       SurfaceTractionIntegrator.cc
       NonLinElasticIntegrator.cc
       NonLinearElasticIntegrator.cc
       TrussIntegrator.cc)
endif(CORE_FOUND)

if(CORESIM_FOUND)
  list(APPEND interface_public_headers 
        SimFEA.h
	apfSimFEA.h
	Elasticity.h
	NonLinElasticity.h
	SimTensorFieldQuery.h)

  list(APPEND interface_src_files 
	apfFunctions.cc 
	SimFEA.cc
	apfSimFEA.cc
	Elasticity.cc 
	NonLinElasticity.cc)
endif(CORESIM_FOUND)

# Add the library to create
add_library(amsi_interface ${interface_src_files} ${interface_public_headers})

# properties for downstream packages
target_include_directories(amsi_interface PUBLIC
					  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
					  $<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(amsi_interface PUBLIC amsi_util)

if(PETSC_FOUND)
  target_include_directories(amsi_interface PUBLIC ${PETSC_INCLUDES})
  if(ZOLTAN_FOUND)
    target_link_libraries(amsi_interface PUBLIC ${ZOLTAN_LIBRARIES})
  endif()
  target_link_libraries(amsi_interface PUBLIC ${PETSC_LIBRARIES})
endif()

if(CORESIM_FOUND)
  target_include_directories(amsi_interface PUBLIC ${CORESIM_INCLUDE_DIRS})
  target_link_libraries(amsi_interface PUBLIC ${CORESIM_LIBRARIES})
endif()

install(TARGETS amsi_interface
	EXPORT amsiinterfaceTargets 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include/amsi)

install(EXPORT amsiinterfaceTargets
	DESTINATION ${ConfigPackageLocation})

install(FILES ${interface_public_headers} DESTINATION include/amsi)
