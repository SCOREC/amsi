#include "amsiUtil.h"
#include "amsiUtilConfig.h"
#include "amsiMPI.h"
#include <pystring.h>
#include <PCU.h>
#include <getopt.h>
#include <iostream>
namespace amsi
{
  FileSystemInfo * fs = NULL;
  bool cnfg_frm_fl = false;
  std::string options_filename;
  static int inited = 0;
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
      if(ln == std::string("@") + std::string(UtilConfigSectionStrings[ii]))
        return ii;
    }
    return -1;
  }
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
        if(fs)
          delete fs;
        fs = new FileSystemInfo(ln);
        describeFSInfo(fs,std::cout);
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
      cnfg_frm_fl = parse_options(argc,argv);
      if(cnfg_frm_fl)
        configureUtilFromFile(options_filename);
      if(rnk > 1)
        amsi::expressOutput(std::cout);
    }
    inited++;
  }
  void freeUtil()
  {
    inited--;
    if(!inited)
    {
      PCU_Comm_Free();
      MPI_Finalize();
    }
  }
}
