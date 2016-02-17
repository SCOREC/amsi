set(interface_public_headers
    amsiInterface.h
    amsiInterfaceConfig.h
    amsiBCDirichlet.h
    amsiBoundaryConditions.h
    amsiDirichletBC.h
    amsiNonlinearAnalysis.h
    amsiNeumannIntegrators.h
    apfBoundaryConditions.h
    apfBoundaryConditions_impl.h
    ElementalSystem.h
    ErrorEstimators.h
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
    FollowForceIntegrator.h
    sim.h
    simAnalysis.h
    simAttributes.h
    simBoundaryConditions.h
    simBoundaryConditions_impl.h
    simWrapper.h
   )

set(interface_src_files
    amsiInterface.cc
    amsiBCDirichlet.cc
    amsiBoundaryConditions.cc
    amsiNonlinearAnalysis.cc
    apfBoundaryConditions.cc
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
    FollowForceIntegrator.cc
    simAnalysis.cc
    simAttributes.cc
    simBoundaryConditions.cc
    simWrapper.cc
   )

find_package(PETSc REQUIRED)
find_package(zoltan)
find_package(Core COMPONENTS Sim)
find_package(SimModSuite REQUIRED)

list(APPEND interface_public_headers PetscLAS.h)
list(APPEND interface_src_files PetscLAS.cc)

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
add_library(interface ${interface_src_files})

# properties for downstream packages
target_include_directories(interface PUBLIC
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(interface PUBLIC util meta)

target_include_directories(interface PUBLIC
  ${PETSC_INCLUDE_DIRS}
  ${CORE_INCLUDE_DIRS}
  ${SIMMODSUITE_INCLUDE_DIR})
target_link_libraries(interface PUBLIC
  ${PETSC_LIBRARIES}
  ${CORE_LIBS}
  ${SIMMODSUITE_LIBS})

install(TARGETS interface
        EXPORT amsiinterfaceTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

install(EXPORT amsiinterfaceTargets
        DESTINATION ${ConfigPackageLocation})

install(FILES ${interface_public_headers} DESTINATION include/amsi)
