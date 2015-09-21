set(interface_public_headers
    amsiInterface.h
    amsiInterfaceConfig.h
    amsiBCDirichlet.h
    ElementalSystem.h
    MatrixUtil.h
    FullLULAS.h
    LAS.h
    FEA.h
    Solvers.h
    TensorFieldQuery.h
    NonFiniteElement.h
    ConvenienceFunctions.h
    ElementalSystem.h
    NeumannIntegrator.h
    apfFunctions.h
    apfFEA.h
    LinearElasticIntegrator.h
    StressStrainIntegrator.h
    SurfaceTractionIntegrator.h
    NonLinElasticIntegrator.h
    NonLinearElasticIntegrator.h
    TrussIntegrator.h
   )

set(interface_src_files
    amsiInterface.cc
    amsiBCDirichlet.cc
    MatrixUtil.h
    FullLULAS.cc
    FEA.cc
    Solvers.cc
    NonFiniteElement.cc
    ConvenienceFunctions.cc
    ElementalSystem.cc
    NeumannIntegrator.cc
    apfFunctions.cc
    apfFEA.cc
    LinearElasticIntegrator.cc
    StressStrainIntegrator.cc
    SurfaceTractionIntegrator.cc
    NonLinElasticIntegrator.cc
    NonLinearElasticIntegrator.cc
    TrussIntegrator.cc
   )

find_package(MPI REQUIRED)
find_package(ZOLTAN)
find_package(PETSc)
find_package(CoreSim)

# petsc is optional
if(PETSC_FOUND)
  list(APPEND interface_public_headers PetscLAS.h)
  list(APPEND interface_src_files PetscLAS.cc)
  include_directories(${PETSC_INCLUDES})
endif(PETSC_FOUND)

# will only be found if we're doing a simmetrix build
if(SIMMODSUITE_FOUND)
  include(SimmetrixFiles.cmake)
  list(APPEND interface_public_headers 
        SimFEA.h
	apfSimFEA.h
	Elasticity.h
	NonLinElasticity.h
	SimTensorFieldQuery.h)

  list(APPEND interface_src_files 
	SimFEA.cc
	apfSimFEA.cc
	Elasticity.cc 
	NonLinElasticity.cc)
endif(SIMMODSUITE_FOUND)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiInterfaceConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiInterfaceConfig.h
               @ONLY)

# Add the library to create
add_library(amsi_interface ${interface_src_files})

# properties for downstream packages
target_include_directories(amsi_interface PUBLIC
					  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
					  $<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(amsi_interface PUBLIC amsi_util amsi_meta)

if(PETSC_FOUND)
  target_include_directories(amsi_interface PUBLIC ${PETSC_INCLUDE_DIRS})
  target_link_libraries(amsi_interface PUBLIC ${PETSC_LIBRARIES})
endif()

target_include_directories(amsi_interface PUBLIC ${CORE_INCLUDE_DIRS})
target_link_libraries(amsi_interface PUBLIC ${CORE_LIBRARIES})

install(TARGETS amsi_interface
	EXPORT amsiinterfaceTargets 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include/amsi)

install(EXPORT amsiinterfaceTargets
	DESTINATION ${ConfigPackageLocation})

install(FILES ${interface_public_headers} DESTINATION include/amsi)
