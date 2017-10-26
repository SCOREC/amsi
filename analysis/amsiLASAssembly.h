#ifndef AMSI_LAS_ASSEMBLY_H_
#define AMSI_LAS_ASSEMBLY_H_
#include "amsiLAS2.h"
#include "amsiElementalSystem.h"
namespace amsi
{
  void assemble(las::LasOps * ops, las::Mat * k, las::Vec * f, ElementalSystem2 * es);
}
#endif
