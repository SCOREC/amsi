#ifndef SIM_CLASSIFIED_H_
#define SIM_CLASSIFIED_H_
#include "simIterator.h"
namespace amsi
{
  SimIterator beginClassified(apf::Mesh * msh, apf::ModelEntity * mdl_ent, int dm);
  SimIterator endClassified(const SimIterator & bgn);
}
#endif
