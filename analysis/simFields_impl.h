#ifndef SIM_FIELDS_IMPL_H_
#define SIM_FIELDS_IMPL_H_
#include "simAttributes.h"
#include "apfFields.h"
#include <algorithm>
namespace amsi
{
  template<class O>
    void buildFieldsFromSim(pACase cs,
                            apf::Mesh * msh,
                            O out)
  {
    std::vector<pANode> flds;
    getTypeNodes((pANode)cs,"field",std::back_inserter(flds));
    for(auto fld = flds.begin(); fld != flds.end(); ++fld)
    {
      FieldSpec spc;
      buildFieldSpecFromSimNode((pANode)*fld,spc);
      *out++ = buildFieldFromSpec(msh,&spc);
      if(spc.tp == FieldType::delta)
      {
        spc.tp = FieldType::full;
        *out++ = buildFieldFromSpec(msh,&spc);
      }
    }
  }
  template <class I, class O>
    void buildNumberingsFromSim(pACase cs, I fld_bgn, I fld_nd, O out)
  {
    std::vector<pANode> flds;
    getTypeNodes(cs,"field",std::back_inserter(flds));
    for(auto fld = flds.begin(); fld != flds.end(); ++fld)
    {
      pANode sl = AttNode_childByType((pANode)*fld,"solution");
      if(AttInfoInt_value((pAttInfoInt)sl))
      {
        FieldSpec spc;
        buildFieldSpecFromSimNode((pANode)*fld,spc);
        std::string nm = composeFieldName(spc.nt,spc.tp,spc.nm);
        auto apf_fld = std::find_if(fld_bgn,fld_nd,[&](apf::Field *& f)->bool { return nm == apf::getName(f); });
        if(apf_fld != fld_nd)
          *out++ = apf::createNumbering(*apf_fld);
      }
    }
  }
}
#endif
