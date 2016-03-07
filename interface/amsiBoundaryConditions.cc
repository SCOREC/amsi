#include "amsiBoundaryConditions.h"
#include "amsiFields.h"
namespace amsi
{
  int numBCComponents(int tp, int sbtp)
  {
    if(tp == DIRICHLET)
      return numDirichletComponents(sbtp);
    else if(tp == NEUMANN)
      return numNeumannComponents(sbtp);
    else
      return 0;
  }
  int numDirichletComponents(int tp)
  {
    switch(tp)
    {
    case DISPLACEMENT:
      return 3;
    default:
      return 0;
    }
  }
  int numNeumannComponents(int tp)
  {
    switch(tp)
    {
    case SURFACE_TRACTION:
      return 3;
    case NORMAL_PRESSURE:
      return 1;
    default:
      return 0;
    }
  }
}
