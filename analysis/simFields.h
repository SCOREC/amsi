#ifndef SIM_FIELDS_H_
#define SIM_FIELDS_H_
#include "sim.h"
#include "apf.h"
namespace amsi
{
  struct FieldSpec;
  void buildFieldSpecFromSimNode(pANode fld_nd, FieldSpec & out);
  template <class O>
    void buildFieldsFromSim(pACase cs,
                            apf::Mesh * msh,
                            O out);
  template <class I, class O>
    void buildNumberingsFromSim(pACase cs,
                                I fld_bgn,
                                I fld_nd,
                                O out);
}
#include "simFields_impl.h"
#endif
