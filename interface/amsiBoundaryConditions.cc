#include "amsiBoundaryConditions.h"
namespace amsi
{
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
}
