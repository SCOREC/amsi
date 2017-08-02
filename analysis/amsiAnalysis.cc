#include "amsiAnalysis.h"
#include "sim.h"
#include <amsiUtil.h>
#include <gmi_null.h>
#include <gmi_sim.h>
#include <pystring.h>
#include <cassert>
namespace amsi
{
  #define ANALYSIS_OPTIONS(OP) OP(petsc), OP(petsc_options), OP(simmetrix), OP(simmetrix_license), OP(num_analysis_options)
  enum AnalysisOptions{ANALYSIS_OPTIONS(MAKE_ENUM_OP)};
  const char * const getAnalysisOptionString(int ii)
  {
    static const char * const AnalysisOptionStrings[] = {ANALYSIS_OPTIONS(MAKE_STRING_OP)};
    assert(ii < num_analysis_options);
    return AnalysisOptionStrings[ii];
  }
  int getAnalysisOption(const std::string & optstr)
  {
    for(int ii = 0 ; ii < num_analysis_options; ++ii)
    {
      if(optstr == std::string(getAnalysisOptionString(ii)))
        return ii;
    }
    return -1;
  }
  const char * const getAnalysisSectionString(int ii)
  {
    static const char * const AnalysisConfigSectionStrings[] = {ANALYSIS_CONFIG_SECTIONS(MAKE_STRING_OP)};
    assert(ii < num_analysis_config_sections);
    return AnalysisConfigSectionStrings[ii];
  }
  int parseAnalysisSection(const std::string & ln)
  {
    for(int ii = 0; ii < num_analysis_config_sections; ++ii)
    {
      if(ln == std::string("@")+std::string(getAnalysisSectionString(ii)))
        return ii;
    }
    return -1;
  }
# ifdef PETSC
  bool use_petsc = false;
  std::string petsc_options_file("");
  void usePetsc(const std::string & petsc_opts)
  {
    use_petsc = true;
    petsc_options_file = petsc_opts;
  }
  void petscInit(int argc, char ** argv, MPI_Comm cm)
  {
    PETSC_COMM_WORLD = cm;
    PetscInitialize(&argc,&argv,petsc_options_file.c_str(),PETSC_NULL);
  }
  void petscFree()
  {
    PetscFinalize();
  }
# endif
  bool use_simmetrix = false;
  std::string simmetrix_license_file("");
  void useSimmetrix(const std::string & sim_lic)
  {
    use_simmetrix = true;
    simmetrix_license_file = sim_lic;
  }
  void simmetrixInit(int argc, char ** argv, MPI_Comm cm)
  {
    PMU_setCommunicator(cm);
    Sim_readLicenseFile(simmetrix_license_file.c_str());
    MS_init();
    SimPartitionedMesh_start(0,0);
    //SimModel_start();
    SimDiscrete_start(0);
    SimMeshing_start();
    gmi_sim_start();
    gmi_register_sim();
    //SimField_start();
  }
  void simmetrixFree()
  {
    //SimField_stop();
    gmi_sim_stop();
    SimMeshing_stop();
    SimDiscrete_stop(0);
    //SimModel_stop();
    SimPartitionedMesh_stop();
    MS_exit();
    Sim_unregisterAllKeys();
  }
  void parseAnalysis(std::istream & fl)
  {
    std::string ln;
    std::vector<std::string> tks;
    bool parsing = true;
    while(parsing)
    {
      std::streampos ln_bgn = fl.tellg();
      if(std::getline(fl,ln))
      {
        if(pystring::startswith(ln,std::string("@")))
        {
          fl.seekg(ln_bgn);
          break;
        }
        pystring::partition(ln," ",tks);
        assert(tks.size() == 3);
        int opt = getAnalysisOption(tks[0]);
        switch(opt)
        {
        case petsc:
          use_petsc = string2Bool(tks[2]);
          break;
        case petsc_options:
          petsc_options_file = tks[2];
          break;
        case simmetrix:
          use_simmetrix = string2Bool(tks[2]);
          break;
        case simmetrix_license:
          simmetrix_license_file = tks[2];
          break;
        default:
          std::cerr << "ERROR: Unknown amsi analysis option: " << tks[0] << std::endl;
          break;
        };
      }
      else
        parsing = false;
    }
  }
  void configureAnalysisFromFile(const std::string & fnm)
  {
    std::fstream fl(fnm.c_str(),std::fstream::in);
    assert(fl.is_open());
    std::string line;
    while(std::getline(fl,line))
    {
      line = pystring::strip(line);
      if(pystring::startswith(line,std::string("@")))
      {
        int sctn = parseAnalysisSection(line);
        switch(sctn)
        {
        case analysis:
          parseAnalysis(fl);
          break;
        }
      }
    }
  }
  // call inside of a scale/task main, not in the binary main (preferably)
  void initAnalysis(int argc, char ** argv, MPI_Comm cm)
  {
    initUtil(argc,argv,cm);
    if(configuredFromFile())
      configureAnalysisFromFile(getOptionsFilename());
#   ifdef PETSC
    if(use_petsc)
      petscInit(argc,argv,cm);
#   endif
    if(use_simmetrix)
      simmetrixInit(argc,argv,cm);
    gmi_register_null();
  }
  void freeAnalysis()
  {
    if(use_simmetrix)
      simmetrixFree();
#   ifdef PETSC
    if(use_petsc)
      petscFree();
#   endif
    freeUtil();
  }
}
