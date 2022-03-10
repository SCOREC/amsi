#ifndef AMSI_ANALYSIS_H_
#define AMSI_ANALYSIS_H_
#include "amsiAnalysisConfig.h"
#include "amsiPETScLAS.h"
#include "amsiNonlinearAnalysis.h"
#include "amsiConfigurationOptions.h"
#include "amsiMPI.h"
namespace amsi {
  class Analysis {
    class PetscScopeGuard {
      public:
      [[nodiscard]] PetscScopeGuard(int argc, char** argv,
                                    const std::string& petsc_options_file,
                                    MPI_Comm cm);
      ~PetscScopeGuard();

      private:
      bool initialized_here_{false};
    };

    public:
    Analysis(const AmsiOptions& options, int argc, char** argv, MPI_Comm cm)
        : petsc_scope_guard_(argc, argv, options.petsc_options_file, cm)
    {
    }

    private:
    [[maybe_unused]] PetscScopeGuard petsc_scope_guard_;
  };
  //#ifdef PETSC
  //  void usePetsc(const std::string & petsc_opts);
  //#endif
}  // namespace amsi
#endif
