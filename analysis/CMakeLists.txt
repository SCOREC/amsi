set(interface_public_headers
    amsiInterface.h
    amsiInterfaceConfig.h
    amsiAnalysis.h
    amsiBoundaryConditions.h
    amsiBoundaryConditions_impl.h
    amsiDummyLAS.h
    amsiFiniteElementAnalysis.h
    amsiFields.h
    amsiFields_impl.h
    amsiFEA.h
    amsiLAS.h
    amsiNonlinearAnalysis.h
    amsiNeumannIntegrators.h
    amsiPETScLAS.h
    apfBoundaryConditions.h
    apfBoundaryConditions_impl.h
    apfFields.h
    apfFunctions.h
    apfFunctions_impl.h
    apfWrapper.h
    apfWrapper_impl.h
    apfsimWrapper.h
    ElementalSystem.h
    ErrorEstimators.h
    MatrixUtil.h
    FullLULAS.h
    Solvers.h
    TensorFieldQuery.h
    NonFiniteElement.h
    ConvenienceFunctions.h
    ElementalSystem.h
    apfFEA.h
    LinearElasticIntegrator.h
    StressStrainIntegrator.h
    NonLinElasticIntegrator.h
    NonLinearElasticIntegrator.h
    MatrixUtil.h
    sim.h
    simAnalysis.h
    simAnalysis_impl.h
    simAttributes.h
    simAttributes_impl.h
    simBoundaryConditions.h
    simBoundaryConditions_impl.h
    simFields.h
    simFields_impl.h
    simWrapper.h
    SimFEA.h
    apfSimFEA.h
    Elasticity.h
    NonLinElasticity.h
   )

set(interface_src_files
    amsiInterface.cc
    amsiBoundaryConditions.cc
    amsiFiniteElementAnalysis.cc
    amsiFields.cc
    amsiFEA.cc
    amsiNonlinearAnalysis.cc
    amsiNeumannIntegrators.cc
    amsiPETScLAS.cc
    apfBoundaryConditions.cc
    apfFields.cc
    apfFunctions.cc
    apfWrapper.cc
    apfsimWrapper.cc
    MatrixUtil.h
    FullLULAS.cc
    Solvers.cc
    NonFiniteElement.cc
    ConvenienceFunctions.cc
    ElementalSystem.cc
    apfFEA.cc
    LinearElasticIntegrator.cc
    StressStrainIntegrator.cc
    NonLinElasticIntegrator.cc
    NonLinearElasticIntegrator.cc
    MatrixUtil.cc
    simAnalysis.cc
    simAttributes.cc
    simBoundaryConditions.cc
    simFields.cc
    simWrapper.cc
    SimFEA.cc
    apfSimFEA.cc
    Elasticity.cc
    NonLinElasticity.cc
   )

find_package(PETSc REQUIRED)
find_package(zoltan)
find_package(SCOREC CONFIG REQUIRED)
#find_package(SimModSuite REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiInterfaceConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiInterfaceConfig.h
               @ONLY)

# Add the library to create
add_library(interface ${interface_src_files})

# properties for downstream packages
target_include_directories(interface PUBLIC
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(interface PUBLIC util meta SCOREC::gmi_sim SCOREC::apf_sim SCOREC::ma ${PETSC_LIBRARIES})
target_include_directories(interface PUBLIC ${PETSC_INCLUDE_DIRS})


install(TARGETS interface
        EXPORT amsiinterfaceTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

install(EXPORT amsiinterfaceTargets
        DESTINATION ${ConfigPackageLocation})

install(FILES ${interface_public_headers} DESTINATION include/amsi)
