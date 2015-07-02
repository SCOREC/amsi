/******************************************************************************

  (c) 2014 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#ifndef FULLLULINEARSYSTEMSOLVER_H_
#define FULLLULINEARSYSTEMSOLVER_H_

#include "LAS.h"

#include <cstring> // memset
#include <vector>

namespace amsi {
  namespace Analysis {
  
    class FullLULAS : public LAS 
    {
      double ** matrix;
      std::vector<double> vector;
      std::vector<double> solution;
      int num_rows;

  public:
    FullLULAS(int num_rows);
    ~FullLULAS();

    void Initialize(int,int,int,int*);

    void AddToMatrix(int r, int c, double v) 
    { matrix[r][c] = v; }

    void AddToVector(int r, double v)
    { vector[r] = v; }

    void Solve();
    
    bool Zero()
    { return ZeroMatrix() && ZeroVector(); }

    bool ZeroMatrix()
    { memset(&matrix[0][0],0,num_rows*num_rows); return true; }

    bool ZeroVector()
    { memset(&vector[0],0,num_rows); return true; }

    void GetSolution(double * sol)
    { sol = &solution[0]; }

    void GetVector(double * vec)
    { vec = &vector[0]; }

    void SetVector(double * vec)
    { std::copy(vec, vec + vector.size(), vector.begin()); }

    bool SeperateSolve() {return false;}
};

  }
} 

#endif





