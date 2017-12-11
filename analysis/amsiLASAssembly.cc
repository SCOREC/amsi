#include "amsiLASAssembly.h"
namespace amsi
{
  void assemble(las::LasOps * ops, las::Mat * k, las::Vec * f, ElementalSystem2 * es)
  {
    // assumes that the es vector and matrix are stored contiguously after the first element
    ops->assemble(k,es->nedof(),&(es->dofs(0)),es->nedof(),&(es->dofs(0)),&(es->ke(0,0)));
    ops->assemble(f,es->nedof(),&(es->dofs(0)),&(es->fe(0)));
  }
}
