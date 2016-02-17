#include "amsiNeumannIntegrators.h"
namespace amsi
{
  NeumannIntegrator * buildNeumannIntegrator(apf::Field * fld, int o, BCQuery * qry, int tp)
  {
    switch(tp)
    {
    case SURFACE_TRACTION:
      return new SurfaceTraction(fld,o,qry);
    case PRESSURE:
      return new Pressure(fld,o,qry);
    default:
      return NULL;
    }
  }
}
