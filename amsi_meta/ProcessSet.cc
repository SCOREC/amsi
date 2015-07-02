#include "ProcessSet.h"

#include <iostream>

namespace amsi {

    /// @brief Default constructor for a range ProcessSet, takes low and high ranks
    /// @param l The first process rank in the set
    /// @param h One over the last process rank in the set
    ProcessSet_T<std::pair<int,int> >::ProcessSet_T(int l, int h) :
      val(std::make_pair(l,h))
    {}


    ProcessSet_T<std::pair<int,int> >::ProcessSet_T(std::pair<int,int> r) :
      val(r)
    {}

    /// @brief Constructor taking a std::pair<int,int> range instead of high and low values
    /// @param r The pair representing the range for the process set
    ProcessSet_T<std::pair<int,int> >::ProcessSet_T(std::pair<int,int> & r) :
      val(r)
    {}
    
    /// @brief Retrieve process ranks from the set
    /// @param i The index of the rank the retrieve
    /// @return int The i-th rank in the set
    int ProcessSet_T<std::pair<int,int> >::operator[](int i)
    {
      return i <= (val.second - val.first) ? val.first + i : -1;
    }
    
    /// @brief How many ranks are in the set
    /// @return int The set size
    int ProcessSet_T<std::pair<int,int> >::size()
    {
      //std::cout << val.first << " " << val.second << std::endl;
      return val.second - val.first;
    }
    
    /// @brief Determine whether a rank is in the set
    /// @param p The rank to check
    /// @return bool Whether the rank is in the set or not
    bool ProcessSet_T<std::pair<int,int> >::isIn(int p)
    {
      return p >= val.first && p < val.second;
    }

    /// @brief Extract a subset of this ProcessSet consisting of s ranks
    /// @param s How many ranks to include in the new set
    /// @return ProcessSet* The newly-created ProcessSet
    ProcessSet * ProcessSet_T<std::pair<int,int> >::extract(int s)
    {
      ProcessSet * result = NULL;  

      if(s <= this->size())
      {
	result =  static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >(this->operator[](0),this->operator[](s)));
	
	val.first += s;
      }

      return result;
    }

    int ProcessSet_T<std::pair<int,int> >::indexOf(int r)
    {
      int result = -1;
      if(isIn(r))
	result = r - val.first;
      return result;
    }

} // namespace amsi
