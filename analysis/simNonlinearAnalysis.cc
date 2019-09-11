#include "simNonlinearAnalysis.h"
#include "amsiLASQuery.h"
#include <amsiOperators.h>
#include <string>
namespace amsi
{
  static LASQueryFunc cvg_ops[] = { &LAS::GetVectorNorm,
                                    &LAS::GetSolutionNorm,
                                    &LAS::GetDotNorm };
  static LASQueryFunc ref_ops[][3] = { {&LAS::GetAccumVectorNorm,
                                        &LAS::GetAccumVectorNorm,
                                        &LAS::GetPrevVectorNorm},
                                       {&LAS::GetAccumSolutionNorm,
                                        &LAS::GetAccumSolutionNorm,
                                        &LAS::GetPrevSolutionNorm},
                                       {&LAS::GetAccumDotNorm,
                                        &LAS::GetAccumDotNorm,
                                        &LAS::GetPrevDotNorm} };
  to_R1 * getConvergenceValueOp(int cvg_tp, LAS * las)
  {
    return new LASNormQuery(las,cvg_ops[cvg_tp]);
  }
  to_R1 * getReferenceValueOp(int ref_tp, int cvg_tp, LAS * las)
  {
    if(ref_tp == 0) // absolute
      std::cerr << "ERROR: initial reference is unimplemented." << std::endl;
    return new LASNormQuery(las,ref_ops[cvg_tp][ref_tp]);
  }
  Convergence * buildSimConvergenceOperator(pACase, pAttribute cn, MultiIteration * it, LAS * las)
  {
    Convergence * cnvg = NULL;
    char * tp = Attribute_imageClass(cn);
    if(std::string("linear convergence").compare(tp) == 0)
    {
      return new LinearConvergence();
    }
    if(std::string("nonlinear iteration").compare(tp) == 0)
    {
      pAttribute cvg_tp = Attribute_childByType(cn,"convergence type");
      pAttribute eps_att = Attribute_childByType(cn,"epsilon");
      pAttribute ref_tp = Attribute_childByType(cn,"reference value");
      pAttribute cap_att = Attribute_childByType(cn,"iteration cap");
      int cvg_tp_vl = AttributeInt_value((pAttributeInt)cvg_tp);
      int ref_tp_vl = AttributeInt_value((pAttributeInt)ref_tp);
      to_R1 * cvg_vl = getConvergenceValueOp(cvg_tp_vl,las);
      to_R1 * ref_vl = getReferenceValueOp(ref_tp_vl,cvg_tp_vl,las);
      auto * eps_vl = new SimUpdatingEpsilon((pAttributeDouble)eps_att);
      if(cap_att)
      {
        // lifetime of this iteration is linked to the lifetime of the associated MultiIteration
        Iteration * stop_at_max_iters = new StopAtMaxIters(AttributeInt_value((pAttributeInt)cap_att));
        it->addIteration(stop_at_max_iters);
      }
      cnvg = new UpdatingConvergence<to_R1*,R1_to_R1*,to_R1*>(it,cvg_vl,eps_vl,ref_vl);
    }
    Sim_deleteString(tp);
    return cnvg;
  }
}
