#ifndef SIMBCDIRICHLET_H_
#define SIMBCDIRICHLET_H_
#include <MeshSim.h>
#include "amsiBCDirichlet.h"
namespace amsi
{
  void buildDirichletSpecFromSIM(pGModel model,
                                 const std::string & attribute_name,
                                 const std::string & constraint_name,
                                 int component_offset,
                                 std::vector<DirichletSpecification> & spec);
}
#endif
