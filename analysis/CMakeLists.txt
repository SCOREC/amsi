find_package(PETSc)
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
        amsiElementalSystem.h
        amsiFields.h
        amsiFields_impl.h
        amsiFEA.h
        amsiLAS.h
        amsiLASQuery.h
        amsiLinearElasticConstitutive.h
        amsiNonlinearAnalysis.h
        amsiNeumannIntegrators.h
        amsiPETScLAS.h
        apfBoundaryConditions.h
        apfBoundaryConditions_impl.h
        apfElementalSystem.h
        apfFieldOp.h
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
        MatrixUtil.h
        FullLULAS.h
        Solvers.h
        ElementalSystem.h
        apfFEA.h
        MatrixUtil.h
        sim.h
        simAnalysis.h
        simAnalysis_impl.h
        simAttributes.h
        simAttributes_impl.h
        simBoundaryConditions.h
        simBoundaryConditions_impl.h
        simClassified.h
        simNonlinearAnalysis.h
        simWrapper.h
        )

set(analysis_src_files
        amsiAnalysis.cc
        amsiBoundaryConditions.cc
        amsiDeformation.cc
        amsiFields.cc
        amsiFEA.cc
        amsiLinearElasticConstitutive.cc
        amsiNonlinearAnalysis.cc
        amsiNeumannIntegrators.cc
        amsiPETScLAS.cc
        apfBoundaryConditions.cc
        apfElementalSystem.cc
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
        ElementalSystem.cc
        apfFEA.cc
        MatrixUtil.cc
        simAnalysis.cc
        simAttributes.cc
        simBoundaryConditions.cc
        simClassified.cc
        simNonlinearAnalysis.cc
        simWrapper.cc
        )

# Add the library to create
add_library(analysis STATIC ${analysis_src_files})

# properties for downstream packages
target_include_directories(analysis PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include/amsi>
        ${PETSC_INCLUDE_DIRS})

if (NOT ${BUILD_EXTERNAL})
    find_package(ModelTraits REQUIRED)
endif ()

target_link_libraries(analysis PUBLIC util SCOREC::gmi_sim SCOREC::apf_sim SCOREC::ma ${PETSC_LINK_LIBRARIES} ModelTraits::ModelTraits)

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
