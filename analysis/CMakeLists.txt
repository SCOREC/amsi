find_package(PETSc REQUIRED)
find_package(zoltan)
find_package(SCOREC CONFIG REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiAnalysisConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiAnalysisConfig.h
               @ONLY)

set(analysis_public_headers
    amsiAnalysis.h
    amsiAnalysisConfig.h
    amsiBoundaryConditions.h
    amsiBoundaryConditions_impl.h
    amsiDofHolder.h
    amsiDummyLAS.h
    amsiFiniteElementAnalysis.h
    amsiFields.h
    amsiFields_impl.h
    amsiFEA.h
    amsiLAS.h
    amsiLASQuery.h
    amsiNonlinearAnalysis.h
    amsiNonlinearAnalysis_impl.h
    amsiNeumannIntegrators.h
    amsiPETScLAS.h
    apfBoundaryConditions.h
    apfBoundaryConditions_impl.h
    apfFieldOp.h
    apfFields.h
    apfFunctions.h
    apfFunctions_impl.h
    apfMeasure.h
    apfMeasure_impl.h
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
    simClassified.h
    simFields.h
    simFields_impl.h
    simIterator.h
    simNonlinearAnalysis.h
    simWrapper.h
    SimFEA.h
    apfSimFEA.h
    Elasticity.h
    NonLinElasticity.h
   )

set(analysis_src_files
    amsiAnalysis.cc
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
    apfMeasure.cc
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
    simClassified.cc
    simFields.cc
    simNonlinearAnalysis.cc
    simWrapper.cc
    SimFEA.cc
    apfSimFEA.cc
    Elasticity.cc
    NonLinElasticity.cc
   )

# Add the library to create
add_library(analysis ${analysis_src_files})

# properties for downstream packages
target_include_directories(analysis PUBLIC
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                     $<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(analysis PUBLIC util SCOREC::gmi_sim SCOREC::apf_sim SCOREC::ma ${PETSC_LIBRARIES})
target_include_directories(analysis PUBLIC ${PETSC_INCLUDE_DIRS})


install(TARGETS analysis
        EXPORT amsianalysisTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

install(EXPORT amsianalysisTargets
        DESTINATION ${ConfigPackageLocation})

install(FILES ${analysis_public_headers} DESTINATION include/amsi)
