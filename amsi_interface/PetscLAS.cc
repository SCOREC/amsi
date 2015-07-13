/******************************************************************************
  (c) 2015 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.
*******************************************************************************/

#include "PetscLAS.h"

#include <assert.h>
#include <iostream>
#include <limits>

namespace amsi {
  namespace Analysis {

    void PetscLAS::Reinitialize(int num_local_unknowns,
				int num_global_unknowns,
				int global_offset,
				int * nonzero_per_row)
    {
      Reinitialize(num_local_unknowns,num_global_unknowns,global_offset);
    }

    void PetscLAS::Reinitialize(int num_local_unknowns,
				int num_global_unknowns,
				int global_offset)
    {
      if(num_local_unknowns != vec_high - vec_low ||
	 num_global_unknowns != globalNumEqs ||
	 global_offset != vec_low)
      {
	int N = globalNumEqs = num_global_unknowns;
	int n = num_local_unknowns;
	
	//VecCreateSeq(PETSC_COMM_SELF,N,&vecTemp); //local vector
	
	x_arr = new double[n];                    //local array to store solution
	b_arr = new double[n];
	
	VecCreateMPI(PETSC_COMM_WORLD,n,N,&b);    //parallel vector

	VecSetOption(b, VEC_IGNORE_NEGATIVE_INDICES, PETSC_TRUE);
	
	std::cout << "Local equations = " << n << ", Global Equations = " << N << std::endl;
	MatCreateAIJ(PETSC_COMM_WORLD,n,n,N,N,300,PETSC_NULL,300,PETSC_NULL,&A);
	//MatMPIAIJSetPreallocation(A,300,PETSC_NULL,300,PETSC_NULL); // This is done in previous line
	// todo: remove this once we have preallocation working correctly
	MatSetOption(A, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE);
	
	KSPCreate(PETSC_COMM_WORLD,&solver);
	VecDuplicate(b,&x);
	
	VecGetOwnershipRange(b,&vec_low,&vec_high);
	MatGetOwnershipRange(A,&mat_low,&mat_high);

	std::cout << "Vector ownership range: " << vec_low << "-" << vec_high << std::endl;
	std::cout << "Matrix ownership range: " << mat_low << "-" << mat_high << std::endl;
	std::cout << "(Re)initialized the matrix, vectors and the solver" << std::endl;
      }
    }

  /**
   *@brief Add a value to the current value at the given location in the matrix in the Linear System to be solved.
   * 
   *@param[in] The row at which to ADD the value (1-indexed).
   *@param[in] The column at which to ADD the value (1-indexed).
   *@param[in] The value to insert into the matrix.
   */
    void PetscLAS::AddToMatrix(int row, int col, double value)
    {
      MatSetValues(A,1,&row,1,&col,&value,ADD_VALUES);
    }

    void  PetscLAS::AddToMatrix(int num_rows, int * rows, int num_cols, int * cols, double * values)
    {
      MatSetValues(A,num_rows,rows,num_cols,cols,values,ADD_VALUES);
    }

    /**
     *@brief Add a value to the current value at the given location in the vector in the Linear System to be solved.
     *
     *@param[in] The row of the RHS vector into which to ADD the value.
     *@param[in] The value to ADD to the vector.
     *
     *@todo Construct a test case.
     */
    void PetscLAS::AddToVector(int row , double value)
    {
      if(!b_addMode)
      {
	VecAssemblyBegin(b);
	VecAssemblyEnd(b);
	b_addMode = true;
      }
      VecSetValue(b,row,static_cast<PetscScalar>(value),ADD_VALUES);
      b_assembled = false;
    }

    void PetscLAS::AddToVector(int num_rows, int * rows, double * values)
    {
      if(!b_addMode)
      {
	VecAssemblyBegin(b);
	VecAssemblyEnd(b);
	b_addMode = true;
      }
      VecSetValues(b,num_rows,rows,static_cast<PetscScalar*>(values),ADD_VALUES);
      b_assembled = false;
    }

  /**
   *@brief Solve the Linear System.
   */
    void PetscLAS::Solve()
    {
      if (!b_assembled)
      {
	VecAssemblyBegin(b);
	VecAssemblyEnd(b);
	b_assembled = true;
      }
      
      MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
      MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
      
      KSPSetOperators(solver,A,A);
      KSPSetFromOptions(solver);
      /*
      MatView(A, PETSC_VIEWER_STDOUT_WORLD);
      VecView(b, PETSC_VIEWER_STDOUT_WORLD);
      */
      // solve the system
      KSPSolve(solver,b,x);

      /*
      VecView(x,PETSC_VIEWER_STDOUT_WORLD);
      */
    }
    
    /**
     *@brief Zero out the Matrix and Vector associated with the linear system, without changing their underlying nonzero structures.
     *
     *@todo Construct a test case.
     */
    bool PetscLAS::Zero()
    {
      return (ZeroVector() && ZeroMatrix());
    }
    
    /**
     *@brief Retrieve a pointer to an array of doubles containing the solution to the linear system.
     *
     *@todo Construct a test case. Prevent the code from being executed prior to a solution being produced.
     */
    void PetscLAS::GetSolution(double *& sol)
    {
      PetscScalar * X;
      VecGetArray(x,&X);

      int n = vec_high-vec_low;
      memcpy(x_arr,(double*)X,n*sizeof(PetscScalar));
      VecRestoreArray(x,&X);
      
      sol = &x_arr[0];
    }

    /**
     *@brief Zero out the associated Matrix, should be collective on the Matrix.
     *
     *@return Whether or not the matrix was successfully zeroed.
     *
     *@todo Construct a test case in a parallel environment in order to assure the operation is collective.
     */
    bool PetscLAS::ZeroMatrix()
    {
      // logically collective on A
      return (MatZeroEntries(A) == 0);
    }


    /**
     *@brief Zero out the associated Vector, should be collective on the Vector. 
     *
     *@return Whether or not the vector was successfully zeroed.
     *
     *@todo Construct a test case in a parallel environment in order to asufe the operation is collective.
     */
    bool PetscLAS::ZeroVector()
    {
      if(b_addMode)
      {
	VecAssemblyBegin(b);
    	VecAssemblyEnd(b);
      }
      
      b_assembled = false;
      //b_addMode = false;
      
      VecZeroEntries(b);
      return true;
    }


    /**
     *@brief Retrieve an array of doubles which is a serial copy of the associated vector.
     *
     *@return A pointer to an array of \c doubles representing the associated vector.
     *
     *@todo Construct a test case.
     */
    void PetscLAS::GetVector(double * & vec)
    {
      PetscScalar * temp;
      
      VecScatter ctx;
      
      VecScatterCreateToAll(b,&ctx,&vecTemp);
      VecScatterBegin(ctx,b,vecTemp,INSERT_VALUES,SCATTER_FORWARD);
      VecScatterEnd(ctx,b,vecTemp,INSERT_VALUES,SCATTER_FORWARD);
      
      VecGetArray(vecTemp,&temp);
      memcpy(b_arr,(double*)temp,globalNumEqs*sizeof(PetscScalar));
      VecRestoreArray(vecTemp,&temp);
      
      VecScatterDestroy(&ctx);
      VecZeroEntries(vecTemp);
      
      vec = b_arr;
    }


    /**
     *@brief Overwrite the values in the associated vector with those from the provided array.
     *
     *@param[in] An array of the correct length to update the associated vector.
     *
     *@todo Construct a test case.
     */
    void PetscLAS::SetVector(const double * vec)
    {
      if(!b_addMode)
      {
	VecAssemblyBegin(b);
	VecAssemblyEnd(b);
      }
      
      // This should allow us to update the entire vector at once without the need to generate the array as below, still UNTESTED
      VecSetBlockSize(b,globalNumEqs);
      int zero = 0;
      VecSetValuesBlocked(b,1,&zero,vec,INSERT_VALUES);

      b_assembled = false;
      b_addMode = false;
    }

    void PetscLAS::GetVectorNorm(double & residual_norm)
    {
      // Petsc vector norm isn't working for some reason

      double result = 0.0;      
      VecNorm(b,NORM_2,&result);
      /*

      // So we compute our own norm
      PetscScalar * B;
      VecGetArray(b,&B);

      // Sum up local values

      int n = vec_high-vec_low;
      for(int ii = 0; ii < n; ii++)
	result += B[ii]*B[ii];

      VecRestoreArray(b,&B);
      */
      // shouldn't need to do this...
      //MPI_Allreduce(&result,&result,1,MPI_DOUBLE,MPI_MAX,PETSC_COMM_WORLD);
      //result = sqrt(result);
      residual_norm = result;
    }
    
    /**
     *@brief Destructor.
     */
    PetscLAS::~PetscLAS()
    {
      MatDestroy(&A);
      VecDestroy(&x);
      VecDestroy(&b);
      VecDestroy(&vecTemp);
      KSPDestroy(&solver);
      
      delete[] x_arr;
      delete[] b_arr;
    }

    /**
     *@brief Constructor.
     *
     *@param[in] The number of global equations across the linear system.
     *@param[in] The number of local equations associated with the local system.
     */
    PetscLAS::PetscLAS ( int N, int n ) : 
      b_assembled(true), 
      b_addMode(true),
      globalNumEqs(N)
    {}

    /**
     *@brief Print the associated matrix to the specified output.
     *
     *@param[in] Currently this does not effect where the matrix is printed.
     */
    void PetscLAS::PrintMatrix(std::ostream &)
    {
      MatView(A, PETSC_VIEWER_STDOUT_WORLD);
    }


    /**
     *@brief Print the associated vector to standard output.
     *
     *@param[in] Currently this does not effect where the vector is printed.
     */
    void PetscLAS::PrintVector(std::ostream &)
    {
      VecAssemblyBegin(b);
      VecAssemblyEnd(b);
      VecView(b, PETSC_VIEWER_STDOUT_WORLD);
    }    
  }
} // end of namespace SCOREC_Solver

