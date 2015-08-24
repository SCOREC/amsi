#include "amsiMeta.h"

#include <amsiMPI.h>

#include <cassert>
#include <getopt.h>
#include <pystring.h>
#include <fstream>

namespace amsi {

  TaskManager * tm = NULL;
  CommunicationManager * cm = NULL;

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

  
  void amsiMetaInit(int argc, char ** argv)
  {
    amsiUtilInit(argc,argv);
    if(parse_options(argc,argv))
      configureFromFile(options_filename);
  }

  void amsiMetaFree()
  {
    amsiUtilFree();
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
    while(std::getline(file,line))
    {
      line = pystring::strip(line);
      if(parsing_scales)
      {
	if(pystring::startswith(line,std::string("#")))
	{
	  parsing_scales = false;
	  continue;
	}
	pystring::partition(line," ",tokens);
	assert(tokens.size() == 3);
	tm->createTask(tokens[0],atoi(tokens[2].c_str()));
      }
      else
      {
	pystring::partition(line," ",tokens);
	assert(tokens.size() == 3);
	size_t t1 = tm->getTaskID(tokens[0]);
	size_t t2 = tm->getTaskID(tokens[2]);
	assert(t1 && t2);
	cm->defineRelation(t1,t2);
      }     
    }
    if(!tm->lockConfiguration())
      std::cerr << "Could not configure AMSI with supplied file: " << filename << std::endl;
    AMSI_COMM_LOCAL = getLocal()->Comm();
  }
  
}
