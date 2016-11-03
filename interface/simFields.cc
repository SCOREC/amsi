#include "simFields.h"
namespace amsi
{
  void buildFieldSpecFromSimNode(pANode fld_nd, FieldSpec & out)
  {
    pANode nm_nd = AttNode_childByType(fld_nd,"field name");
    pANode nt_nd = AttNode_childByType(fld_nd,"unit");
    pANode tp_nd = AttNode_childByType(fld_nd,"type");
    pANode vt_nd = AttNode_childByType(fld_nd,"value type");
    pANode rd_nd = AttNode_childByType(fld_nd,"order");
    out.nt = AttInfoInt_value((pAttInfoInt)nt_nd);
    out.tp = AttInfoInt_value((pAttInfoInt)tp_nd);
    out.vt = AttInfoInt_value((pAttInfoInt)vt_nd);
    out.rd = AttInfoInt_value((pAttInfoInt)rd_nd);
    out.nm = std::string(AttInfoString_value((pAttInfoString)nm_nd));
  }
}
