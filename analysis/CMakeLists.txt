find_package(PETSC)
#find_package(zoltan)
find_package(SCOREC CONFIG REQUIRED)

# FIXME configured files should be configured into build dir
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiAnalysisConfig.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiAnalysisConfig.h
               @ONLY)

set(analysis_public_headers
    amsiAnalysis.h
    amsiAnalysisConfig.h
    amsiBoundaryConditions.h
    amsiBoundaryConditions_impl.h
    amsiDeformation.h
    amsiDetectOscillation.h
    amsiDetectOscillation_impl.h
    amsiDofHolder.h
    amsiDummyLAS.h
    amsiElasticityFEA.h
    amsiElementalSystem.h
    amsiFiniteElementAnalysis.h
    amsiFields.h
    amsiFields_impl.h
    amsiFEA.h
    amsiLAS.h
    amsiLAS2.h
    amsiLASAssembly.h
    amsiLASQuery.h
    amsiLinearElasticConstitutive.h
    amsiLinearStressStrainIntegrator.h
    amsiNonlinearAnalysis.h
    amsiNeumannIntegrators.h
    amsiPETScLAS.h
    amsiPETScLAS2.h
    apfBoundaryConditions.h
    apfBoundaryConditions_impl.h
    apfElementalSystem.h
    apfFieldOp.h
    apfFields.h
    apfFunctions.h
    apfFunctions_impl.h
    apfMatrixUtil.h
    apfMeasure.h
    apfMeasure_impl.h
    apfMeshIterator.h
    apfMeshUtil.h
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
    NonLinElasticity.h
   )

set(analysis_src_files
    amsiAnalysis.cc
    amsiBoundaryConditions.cc
    amsiDeformation.cc
    amsiElasticityFEA.cc
    amsiFiniteElementAnalysis.cc
    amsiFields.cc
    amsiFEA.cc
    amsiLASAssembly.cc
    amsiLinearElasticConstitutive.cc
    amsiLinearStressStrainIntegrator.cc
    amsiNonlinearAnalysis.cc
    amsiNeumannIntegrators.cc
    amsiPETScLAS.cc
    amsiPETScLAS2.cc
    apfBoundaryConditions.cc
    apfElementalSystem.cc
    apfFields.cc
    apfFunctions.cc
    apfMatrixUtil.cc
    apfMeasure.cc
    apfMeshIterator.cc
    apfMeshUtil.cc
    apfWrapper.cc
    apfsimWrapper.cc
    MatrixUtil.h
    FullLULAS.cc
    Solvers.cc
    NonFiniteElement.cc
    ConvenienceFunctions.cc
    ElementalSystem.cc
    apfFEA.cc
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
    NonLinElasticity.cc
   )

# Add the library to create
add_library(analysis STATIC ${analysis_src_files})

# properties for downstream packages
target_include_directories(analysis PUBLIC
                                     $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                                     $<INSTALL_INTERFACE:include/amsi>
                                     ${PETSC_INCLUDE_DIRS})

target_link_libraries(analysis PUBLIC util SCOREC::gmi_sim SCOREC::apf_sim SCOREC::ma ${PETSC_LIBRARIES})

install(TARGETS analysis
        EXPORT amsianalysisTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

# this allows us to use the package from the
# build tree which is wanted for multiple build configs
export(EXPORT amsianalysisTargets
       FILE "${CMAKE_BINARY_DIR}/amsi/amsianalysisTargets.cmake"
       NAMESPACE amsi::)

install(EXPORT amsianalysisTargets
        DESTINATION ${ConfigPackageLocation}
        NAMESPACE amsi::)

install(FILES ${analysis_public_headers} DESTINATION include/amsi)
# Headers need to be copied to binary directory to enable building against build
# W/O installation
file(COPY ${analysis_public_headers} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
