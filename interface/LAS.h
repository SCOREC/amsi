/******************************************************************************

  (c) 2014 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#ifndef LAS_H_
#define LAS_H_

#include <ostream>

namespace amsi {
  namespace Analysis {

  class LAS {
  public:
    virtual void iter() {};
    virtual void step() {};
    
    /// Initialize the solver and allocate memory for the local matrix portion
    virtual void Reinitialize(int,int,int,int*) {};
    virtual void Reinitialize(int,int,int) {};

    /// add an element val at given row and column in the system
    virtual void AddToMatrix(int row, int col, double value ) = 0;

    /// add a matrix of values to specific locations in the global system, useful for adding elemental system's contributions
    virtual void AddToMatrix(int num_row, int * rows,
			     int num_col, int * cols,
			     double * values) = 0;

    /// add an element in the right hand side
    virtual void AddToVector(int row, double value) = 0;

    /// add multiple elements to the right hand side
    virtual void AddToVector(int num_rows, int * rows, double * values) = 0;
    
    /// solve the system
    virtual void Solve () = 0;

    /// zero the system
    virtual bool Zero() {return false;}

    /// retrieve the solution
    virtual void GetSolution(double *&) = 0;
    virtual void GetSolutionNorm(double &) = 0;
    virtual void GetAccumSolutionNorm(double &) = 0;

    // Toshi below - must also be able to zero each side separately
    virtual bool ZeroMatrix() {return false;}
    virtual bool ZeroVector() {return false;}

    virtual void GetVector(double *&) = 0;
    virtual void SetVector(const double *) = 0;

    virtual void GetVectorNorm(double &) = 0;
    virtual void GetDotNorm(double &) = 0;

    virtual void PrintMatrix(std::ostream &) {};
    virtual void PrintVector(std::ostream &) {};

    virtual ~LAS() {};
  };
  
  } 
} 

#endif