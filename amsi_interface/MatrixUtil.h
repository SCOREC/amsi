/******************************************************************************

  (c) 2014 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#ifndef MATRIXUTIL_H_
#define MATRIXUTIL_H_

namespace amsi {
  namespace Analysis {

    void direct_solver(double ** matrix, 
		       double * vector, 
		       double * solution, 
		       int size);
    
    void free_matrix( double ** matrix);
    
    double ** allocate_matrix(int size);

  }
}
#endif
