#ifndef APF_SIM_FINITE_ELEMENT_ANALYSIS_H_
#define APF_SIM_FINITE_ELEMENT_ANALYSIS_H_
#include "amsiFiniteElementAnalysis.h"
#include "amsiElementalSystem.h"
#include "sim.h"
#include "apf.h"
namespace amsi
{
  class apfSimFiniteElementAnalysis : public Analysis
  {
  protected:
    MPI_Comm cm;
    double t;
    apf::Mesh * msh;
    pMesh prt;
    std::list<ConstrainedTensorField> sol_flds;
  public:
    apfSimFiniteElementAnalysis(pMesh p, pACase pd, MPI_Comm c = AMSI_COMM_SCALE)
      : cm(c)
      , t(0.0)
      , msh()
      , prt(p)
      , sol_flds()
    {
      msh = apf::createMesh(prt);
      buildFieldsFromSim(pd,std::back_inserter(sol_flds));
    }
    virtual void applyDirichletBCs()
    {
      int fxd = 0;
      for(auto fld = bc_flds.begin(); fld != bc_flds.end(); ++fld)
        fxd += fld->applyDirichletBCs();
      AMSI_V1(std::cout << "Dirichlet boundary conditions fixed " << fxd << " local degrees of freedom on all constrained fields" << std::endl;)
    }
    virtual void applyNeumannBCs(LAS * ls)
    {
      for(auto bc_flds.begin(); fld != bc_flds.end(); ++fld)
        fld->applyNeumannBCs(ls);
    }
    virtual void assemblLinearSystem(LAS * ls);
    virtual void update(LAS * ls);
    virtual void updateDOFInfo(long & gbl, long & lcl, long & ffst)
    {
      lcl = 0;
      for(auto fld = bc_flds.begin(); fld != bc_flds.end(); ++fld)
      {
        apf::Numbering * nm = fld->getNumbering();
        lcl += apf::NaiveOrder(nm);
      }
      gbl = comm_sum(lcl,cm);
      ffst = comm_scan(lcl,MPI_SUM,cm);
    }
  };
}
#endif
