/******************************************************************************

  (c) 2014 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#include "FullLULAS.h"
#include "MatrixUtil.h"

namespace amsi {
  namespace Analysis {

    FullLULAS::FullLULAS(int num_rows_in) :
      vector(num_rows_in, 0.0),
      solution(num_rows_in, 0.0),
      num_rows(num_rows_in)
    {
      matrix = allocate_matrix(num_rows);

      memset(&matrix[0][0],0,num_rows*num_rows);
    }
    
    FullLULAS::~FullLULAS ( )
    {
      free_matrix(matrix);
    }
    
    void FullLULAS::Solve()
    {
      direct_solver(matrix, &vector[0], &solution[0], solution.size());
    }

  }
}







