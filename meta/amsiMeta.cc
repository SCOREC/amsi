#include "amsiMeta.h"
#include <amsiFileSys.h>
#include <amsiMPI.h>
#include <amsiReporter.h>
#include <cassert>
#include <getopt.h>
#include <pystring.h>
#include <fstream>
namespace amsi {

  TaskManager * tm = NULL;
  CommunicationManager * cm = NULL;
  FileSystemInfo * fs = NULL;

  std::string options_filename;
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
  
  void amsiMetaInit::amsiInit(int argc, char ** argv)
  {
    amsiUtilInit::amsiInit(argc,argv);
    if(parse_options(argc,argv))
      configureFromFile(options_filename);
  }

  void amsiMetaInit::amsiFree()
  {
    amsiUtilInit::amsiFree();
  }

  void configureFromFile(const std::string & filename)
  {
    std::fstream file(filename.c_str(),std::fstream::in);
    assert(file.is_open());

    tm = new TaskManager(AMSI_COMM_WORLD);
    cm = new CommunicationManager();
    
    std::vector<std::string> tokens;
    std::string line;
    bool parsing_scales = true;
    bool parsing_relations = false;
    bool parsing_configure = false;
    while(std::getline(file,line))
    {
      line = pystring::strip(line);
      if(parsing_scales)
      {
	if(pystring::startswith(line,std::string("#")))
	{
	  parsing_scales = false;
	  parsing_relations = true;
	  continue;
	}
	pystring::partition(line," ",tokens);
	assert(tokens.size() == 3);
	tm->createTask(tokens[0],atoi(tokens[2].c_str()));
      }
      else if(parsing_relations)
      {
	if(pystring::startswith(line,std::string("#")))
	{
	  parsing_relations=false;
	  parsing_configure=true;
	  continue;
	}
	pystring::partition(line," ",tokens);
	assert(tokens.size() == 3);
	size_t t1 = tm->getTaskID(tokens[0]);
	size_t t2 = tm->getTaskID(tokens[2]);
	assert(t1 && t2);
	cm->defineRelation(t1,t2);
      }
      else if(parsing_configure)
      {
	if(fs)
	  delete fs;
	fs = new FileSystemInfo(line);
	printInfo(fs,std::cout);
      }
    }
    if(!tm->lockConfiguration())
      std::cerr << "Could not configure AMSI with supplied file: " << filename << std::endl;
    AMSI_COMM_LOCAL = getLocal()->comm();
  }
  
}
