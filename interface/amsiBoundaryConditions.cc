#include "amsiBoundaryConditions.h"
#include "amsiFields.h"
namespace amsi
{
  int numBCComponents(int tp, int sbtp)
  {
    if(tp == BCType::dirichlet)
      return numDirichletComponents(sbtp);
    else if(tp == BCType::neumann)
      return numNeumannComponents(sbtp);
    else
      return 0;
  }
  int numDirichletComponents(int tp)
  {
    switch(tp)
    {
    case FieldUnit::unitless:
    case FieldUnit::displacement:
      return 3;
    default:
      return 0;
    }
  }
  int numNeumannComponents(int tp)
  {
    switch(tp)
    {
    case NeuBCType::traction:
      return 3;
    case NeuBCType::pressure:
      return 1;
    default:
      return 0;
    }
  }
}
