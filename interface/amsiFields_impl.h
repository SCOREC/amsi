#include "simWrapper.h"
#include "apfNumbering.h"
#include <algorithm>
#include <pystring.h>
namespace amsi
{
  template<class O>
    void buildFieldsFromSim(pACase cs,
                            apf::Mesh * msh,
                            O out)
  {
    std::vector<pAttribute> flds;
    cutPaste<pAttribute>(AttCase_attributes(cs,"field"),std::back_inserter(flds));
    for(auto fld = flds.begin(); fld != flds.end(); ++fld)
    {
      pAttribute nm_att = Attribute_childByType(*fld,"field name");
      pAttribute nt_att = Attribute_childByType(*fld,"unit");
      pAttribute tp_att = Attribute_childByType(*fld,"type");
      int tp = AttributeInt_value((pAttributeInt)tp_att);
      pAttribute vtp_att = Attribute_childByType(*fld,"value type");
      pAttribute ord_att = Attribute_childByType(*fld,"order");
      std::vector<std::string> nm_vc(3);
      nm_vc[0] = std::string(fieldUnitString(AttributeInt_value((pAttributeInt)nt_att)));
      nm_vc[1] = std::string(fieldTypeString(tp));
      nm_vc[2] = std::string(AttributeString_value((pAttributeString)nm_att));
      std::string nm = pystring::join("_",nm_vc);
      int vtp = AttributeInt_value((pAttributeInt)vtp_att);
      int o = AttributeInt_value((pAttributeInt)ord_att);
      *out++ = apf::createLagrangeField(msh,nm.c_str(),vtp,o);
      if(tp == DELTA)
      {
        std::vector<std::string> fl_nm_vc(3);
        fl_nm_vc[0] = nm_vc[0];
        fl_nm_vc[1] = std::string(fieldTypeString(FULL));
        fl_nm_vc[2] = nm_vc[2];
        std::string fl_nm = pystring::join("_",fl_nm_vc);
        *out++ = apf::createLagrangeField(msh,fl_nm.c_str(),vtp,0);
      }
    }
  }
  template <class I, class O>
    void buildNumberingsFromSim(pGModel mdl, I fld_bgn, I fld_nd, O out)
  {
    std::vector<pAttribute> flds;
    cutPaste<pAttribute>(GM_attributes(mdl,"field"),std::back_inserter(flds));
    for(auto fld = flds.begin(); fld != flds.end(); ++fld)
    {
      pAttribute sol_att = Attribute_childByType(*fld,"solution");
      if(sol_att)
      {
        pAttribute nm_att = Attribute_childByType(*fld,"name");
        std::string nm(AttributeString_value((pAttributeString)nm_att));
        auto apf_fld = std::find_if(fld_bgn,fld_nd,[&](apf::Field *& f)->bool { return nm == decodeFieldName(f); });
        if(apf_fld != fld_nd)
        {
          out = apf::createNumbering(*apf_fld);
          ++out;
        }
      }
    }
  }
}
