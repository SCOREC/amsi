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
  Convergence * buildSimConvergenceOperator(pACase cs, pANode cn, Iteration * it, LAS * las)
  {
    Convergence * cnvg = NULL;
    char * tp = AttNode_imageClass(cn);
    if(std::string("linear convergence").compare(tp) == 0)
    {
      return new LinearConvergence();
    }
    if(std::string("nonlinear iteration").compare(tp) == 0)
    {
      pANode cvg_tp = AttNode_childByType(cn,"convergence type");
      pANode ref_tp = AttNode_childByType(cn,"reference value");
      pANode eps_tp = AttNode_childByType(cn,"epsilon");
      int cvg_tp_vl = AttInfoInt_value((pAttInfoInt)cvg_tp);
      int ref_tp_vl = AttInfoInt_value((pAttInfoInt)ref_tp);
      to_R1 * cvg_vl = getConvergenceValueOp(cvg_tp_vl,las);
      to_R1 * ref_vl = getReferenceValueOp(ref_tp_vl,cvg_tp_vl,las);
      R1_to_R1 * eps_vl = new SimUpdatingEpsilon((pAttInfoDouble)eps_tp);
      cnvg = new UpdatingConvergence<to_R1*,R1_to_R1*,to_R1*>(it,cvg_vl,eps_vl,ref_vl);
    }
    Sim_deleteString(tp);
    return cnvg;
  }
}
