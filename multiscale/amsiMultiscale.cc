#include "amsiMultiscale.h"
#include <amsiUtil.h>
#include <cassert>
#include <getopt.h>
#include <pystring.h>
#include <fstream>
namespace amsi
{
  static TaskManager * tm = NULL;
  static CommunicationManager * cm = NULL;
  TaskManager * getScaleManager()
  {
    return tm;
  }
  CommunicationManager * getMultiscaleManager()
  {
    return cm;
  }
  static int parseMultiscaleSection(const std::string & ln)
  {
    for(int ii = 0; ii < num_multiscale_config_sections; ++ii)
    {
      if(ln == std::string("@")+std::string(MultiscaleConfigSectionStrings[ii]))
        return ii;
    }
    return -1;
  }
  static void parseScales(std::istream & fl)
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
        tm->createTask(tks[0],atoi(tks[2].c_str()));
      }
      else
        parsing = false;
    }
  }
  static void parseRelations(std::istream & fl)
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
        size_t t1 = tm->getTaskID(tks[0]);
        size_t t2 = tm->getTaskID(tks[2]);
        assert(t1 && t2);
        cm->defineRelation(t1,t2);
      }
      else
        parsing = false;
    }
  }
  static void parseAllocator(std::istream & fl)
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
        //pystring::partition(ln," ",tks);
        pystring::split(ln, tks);
        // do nothing this is the default allocator
        if (tks[0] == "ExclusiveProcess")
        {
        }
        else if (tks[0] == "Strided")
        {
          assert(tks.size() == 3);
          int size = 0;
          int stride = std::atoi(tks[2].c_str());
          size_t strided_task_id = tm->getTaskID(tks[1], false);
          assert(strided_task_id);
          MPI_Comm_size(AMSI_COMM_WORLD, &size);
          ProcessAllocator * allocator = static_cast<ProcessAllocator*>(new 
              StridedProcessAllocator(size, stride, strided_task_id));
          tm->setProcessAllocator(allocator);
        }
        else
        {
          std::cerr<<"The Process allocator " << tks[0] <<" is not a valid option.\n";
          std::cerr<<"Please use ExclusiveProcess with no arguments, or Strided with the stride task name and a stide size argument\n";
          MPI_Abort(AMSI_COMM_WORLD, 1);
        }
      }
      else
        parsing = false;
    }
  }
  void configureMultiscaleFromFile(const std::string & filename)
  {
    std::fstream file(filename.c_str(),std::fstream::in);
    assert(file.is_open());
    tm = new TaskManager(AMSI_COMM_WORLD);
    cm = new CommunicationManager();
    std::string line;
    while(std::getline(file,line))
    {
      line = pystring::strip(line);
      std::cout<<line<<std::endl;
      if(pystring::startswith(line,std::string("@")))
      {
        int sctn = parseMultiscaleSection(line);
        switch(sctn)
        {
        case allocator:
          parseAllocator(file);
          break;
        case scales:
          parseScales(file);
          break;
        case relations:
          parseRelations(file);
          break;
        }
      }
    }
    if(!tm->lockConfiguration())
      std::cerr << "ERROR: AMSI multiscale cannot configure with supplied file: " << filename << std::endl;
    AMSI_COMM_SCALE = getLocal()->comm();
  }
  void initMultiscale(int argc, char ** argv, MPI_Comm cm)
  {
    initUtil(argc,argv,cm);
    if(configuredFromFile())
      configureMultiscaleFromFile(getOptionsFilename());
    ControlService * cs = ControlService::Instance();
    cs->SetTaskManager(amsi::tm);
    cs->SetCommunicationManager(amsi::cm);
  }
  void freeMultiscale()
  {
    freeUtil();
  }
}
