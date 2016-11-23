#include "amsiFiniteElementAnalysis.h"
namespace amsi
{
  void assembleDOFs(LAS * las,
                    int nedofs,
                    int * dofids,
                    bool * fxd,
                    double * dofs,
                    double * ke,
                    double * fe,
                    bool induced_forces)
  {
    if(ke != NULL)
    {
      if(!induced_forces)
      {
        double * bf = new double[nedofs];
        for(int ii = 0; ii < nedofs; ii++)
        {
          if(!fxd[ii])
          {
            //const int & gi = dofids[ii];
            for(int jj = 0; jj < nedofs; jj++)
            {
              const double & val = ke[ii*nedofs + jj];
              double jval = dofs[jj];
              if(jval != 0.0)
                bf[ii] = -val * jval;
            }
          }
        }
        las->AddToVector(nedofs,dofids,&bf[0]);
        delete [] bf;
      }
      las->AddToMatrix(nedofs,dofids,nedofs,dofids,&ke[0]);
    }
    las->AddToVector(nedofs,dofids,&fe[0]);
  }
}
