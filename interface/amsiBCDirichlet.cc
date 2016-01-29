#include "amsiBCDirichlet.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>

namespace amsi {

  void buildDirichletSpecFromStream(std::istream & in,
				    std::vector<DirichletSpecification> & spec)
  {
    std::vector<std::string> tokens;
    std::string line;
    while(std::getline(in,line))
    {
      // strip any leading/trailing whitespace
      line = pystring::strip(line);
      pystring::partition(line," ",tokens);
      assert(tokens.size() == 4);
      DirichletSpecification temp = {atoi(tokens[0].c_str()),  // type
				     atoi(tokens[1].c_str()),  // tag
				     atoi(tokens[2].c_str()),  // component
				     atof(tokens[3].c_str())}; // value      
      spec.push_back(temp);
    }
  }
  
}
