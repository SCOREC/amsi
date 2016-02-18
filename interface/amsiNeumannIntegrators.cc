#include "amsiNeumannIntegrators.h"
namespace amsi
{
  NeumannIntegrator * buildNeumannIntegrator(LAS * las, apf::Field * fld, int o, BCQuery * qry, int tp, double t)
  {
    switch(tp)
    {
    case SURFACE_TRACTION:
      return new SurfaceTraction(las,fld,o,qry,t);
    case FOLLOW_FORCE:
      return new Pressure(las,fld,o,qry,t);
    default:
      return NULL;
    }
  }
}
