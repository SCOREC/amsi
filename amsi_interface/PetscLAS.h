/******************************************************************************

  (c) 2014 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#ifndef PETSCITERATIVESOLVER_H_
#define PETSCITERATIVESOLVER_H_

#include "LAS.h"

#include <petscksp.h>
#include <petscmat.h>

#include <vector>

namespace amsi {
  namespace Analysis {

  class PetscLAS: public LAS
  {
  public:
    PetscLAS(int,int);

    void Reinitialize(int, int, int, int*);
    void Reinitialize(int, int, int);

    void AddToMatrix(int,int,double);
    void AddToMatrix(int,int*,int,int*,double*);
    
    void AddToVector(int,double);
    void AddToVector(int,int*,double*);

    void Solve();

    bool Zero();

    bool ZeroMatrix();
    bool ZeroVector();

    void GetVector(double *&);
    void SetVector(const double *);

    void GetVectorNorm(double &);

    void GetSolution(double *&);
    void GetSolutionNorm(double & n) {};

    void PrintMatrix(std::ostream &);
    void PrintVector(std::ostream &);

    ~PetscLAS();

  private:
    Mat A;
    Vec x;
    Vec b;
    Vec vecTemp;
    double * x_arr, * b_arr;
    int globalNumEqs;
    int vec_low;
    int vec_high;
    int mat_low;
    int mat_high;
    bool b_assembled, b_addMode;
    KSP solver;
  };

  }
} 

#endif
