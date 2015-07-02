/******************************************************************************

  (c) 2014 Scientific Computation Research Center,
      Rensselaer Polytechnic Institute. All rights reserved.

  The LICENSE-SCOREC file included with this distribution describes the terms
  of the SCOREC Non-Commercial License this program is distributed under.

*******************************************************************************/

#ifndef SIMMETRIXTENSORFIELDQUERY_H_
#define SIMMETRIXTENSORFIELDQUERY_H_

#include "TensorFieldQuery.h"

namespace amsi {
  namespace Analysis {

    template <>
      class TensorFieldQueryT<pAttributeTensor1> : public TensorFieldQuery
    {
    private:
      pAttributeTensor1 tensor;
      bool is_space_const, is_time_const;

      int dim;
      int num_args;
      int args_bound;
      bool * is_bound;

      double * args;

      double * last_result;
    public:

    TensorFieldQueryT(pAttributeTensor1 t)
      : tensor(t),
	is_space_const(false),
	is_time_const(false),
	dim(0),
	num_args(0),
	args_bound(0),
	is_bound(0),
	args(0),
	last_result(0)
	{
	  dim = AttributeTensor1_dimension(tensor);
	  last_result = new double[dim];
	  
	  is_space_const = AttributeTensor1_constant(tensor);
	  is_time_const = true;
	  
	  num_args = is_space_const ? 0 : 3;
	  num_args += is_time_const ? 0 : 1;
	  
	  if(num_args > 0)
	  {
	    is_bound = new bool[num_args];
	    args = new double[num_args];
	  }
	}

      double* operator()(int count,...)
      {
	double * result = 0;
	if(num_args == 0)
	{
	  for(int ii = 0; ii < dim; ii++)
	    last_result[ii] = AttributeTensor1_value(tensor,ii);
	  
	  result = last_result;
	}
	else
	{
	  va_list ap;
	  int ii = 0;
	  va_start(ap,count);
	  {
	    while(is_bound[ii]) ii++;
	    bind(ii,va_arg(ap,double));
	  }
	  va_end(ap);
	  
	  bool all_bound = true;
	  for(int ii = 0; ii < num_args; ii++)
	    all_bound = all_bound ? (is_bound[ii] ? true : false) : false;
	  
	  if(all_bound)
	  {
	    if(!is_space_const)
	      AttributeTensor1_evalTensorDS(tensor,&args[0],last_result);
	    
	    if(!is_time_const)
	      AttributeTensor1_evalTensorDT(tensor,args[num_args],last_result);
	    
	    result = last_result;
	  }
	}
	return result;
      }
      
      void bind(int index, double value)
      {
	if(index < num_args && index >= 0 && num_args > 0)
	{
	  args[index] = value;
	  if(is_bound[index] == false)
	  {
	    args_bound++;
	    is_bound[index] = true;
	  }
	}
      }
      
      void clear() 
      {
	memset(last_result,0.0,sizeof(double)*dim);
	
	for(int ii = 0; ii < num_args; ii++)
	  is_bound[ii] = false;
      }

      int bound() {return args_bound;}
      int unbound() {return num_args - args_bound;}
    };
  }
}


#endif
