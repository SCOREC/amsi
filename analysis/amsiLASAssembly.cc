#include "amsiLASAssembly.h"
namespace amsi
{
  void assemble(las::LasOps * ops, las::Mat * k, las::Vec * u, ElementalSystem2 * es)
  {
    ops->assemble(k,es->nedof(),&(es->dofs(0)),es->nedof(),&(es->dofs(0)),&(es->ke(0,0)));
    ops->assemble(u,es->nedof(),&(es->dofs(0)),&(es->fe(0)));
  }
}
