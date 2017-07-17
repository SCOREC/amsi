#include "amsiUtil.h"
#include "amsiUtilConfig.h"
#include "amsiMPI.h"
#include <pystring.h>
#include <PCU.h>
#include <getopt.h>
#include <iostream>
#include <cassert>
namespace amsi
{
  #define UTIL_OPTIONS(OP) OP(results), OP(num_util_options)
  enum UtilOptions{UTIL_OPTIONS(MAKE_ENUM_OP)};
  const char * const getUtilOptionString(int ii)
  {
    static const char * const UtilOptionStrings[] = {UTIL_OPTIONS(MAKE_STRING_OP)};
    assert(ii < num_util_options);
    return UtilOptionStrings[ii];
  }
  int getUtilOption(const std::string & optstr)
  {
    for(int ii = 0; ii < num_util_options; ++ii)
    {
      if(optstr == std::string(getUtilOptionString(ii)))
        return ii;
    }
    return -1;
  }
  const char * const getUtilConfigSectionString(int ii)
  {
    static const char * const UtilConfigSectionStrings[] = {UTIL_CONFIG_SECTIONS(MAKE_STRING_OP)};
    assert(ii < num_util_config_sections);
    return UtilConfigSectionStrings[ii];
  }
  std::string options_filename("");
  const std::string & getOptionsFilename()
  {
    return options_filename;
  }
  bool configuredFromFile()
  {
    return !options_filename.empty();
  }
  bool parse_options(int argc,char ** argv)
  {
    bool have_options = false;
    opterr = 0;
    static struct option long_options[] = {{"amsi",required_argument,0,'a'},{0,0,0,0}};
    int option_index = 0;
    int option = 0;
    while((option = getopt_long(argc,argv,"a:",long_options,&option_index)) != -1)
    {
      if(option == 'a')
      {
        options_filename = optarg;
        have_options = true;
      }
    }
    optind = 0;
    opterr = 1;
    return have_options;
  }
  int parseUtilSection(const std::string & ln)
  {
    for(int ii = 0; ii < num_util_config_sections; ++ii)
    {
      if(ln == std::string("@") + std::string(getUtilConfigSectionString(ii)))
        return ii;
    }
    return -1;
  }
  FileSystemInfo * fs = NULL;
  void parseUtil(std::istream & fl)
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
        int opt = getUtilOption(tks[0]);
        switch(opt)
        {
        case results:
          if(fs)
            delete fs;
          fs = new FileSystemInfo(tks[2]);
          describeFSInfo(fs,std::cout);
          break;
        default:
          std::cerr << "ERROR: Unknown amsi utility option: " << tks[0] << std::endl;
          break;
        };
      }
      else
        parsing = false;
    }
  }
  void configureUtilFromFile(const std::string & flnm)
  {
    std::fstream fl(flnm.c_str(),std::fstream::in);
    assert(fl.is_open());
    std::string ln;
    while(std::getline(fl,ln))
    {
      int sctn = parseUtilSection(ln);
      switch(sctn)
      {
      case util:
        parseUtil(fl);
        break;
      }
    }
  }
  static bool inited = false;
  void initUtil(int argc, char ** argv, MPI_Comm cm)
  {
    if(!inited)
    {
      MPI_Init(&argc,&argv);
      MPI_Comm_dup(cm,&AMSI_COMM_SCALE);
      MPI_Comm_dup(cm,&AMSI_COMM_WORLD);
      PCU_Comm_Init();
      int rnk = -1;
      MPI_Comm_rank(AMSI_COMM_WORLD,&rnk);
      if(rnk > 1)
        amsi::suppressOutput(std::cout);
      if(parse_options(argc,argv))
        configureUtilFromFile(options_filename);
      if(rnk > 1)
        amsi::expressOutput(std::cout);
      inited = true;
    }
  }
  void freeUtil()
  {
    if(inited)
    {
      PCU_Comm_Free();
      MPI_Finalize();
      inited = false;
    }
  }
}
