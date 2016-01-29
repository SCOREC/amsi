#ifndef AMSIBCDIRICHLET_H_
#define AMSIBCDIRICHLET_H_

#include <pystring.h> // from amsi_util
#include <iostream>
#include <vector>

namespace amsi {

  struct DirichletSpecification
  {
    int type;      // the type of geometric entity (vertex,edge,face,region)
    int tag;       // the specific entity
    int component; // which component of the field is fixed
    double value;  // the value to which the component is fixed
  };
  
  void buildDirichletSpecFromStream(std::istream & in,
                                    std::vector<DirichletSpecification> & spec);

}

#endif
