#include "amsiProcessSet.h"
#include <iostream>
#include <cassert>
namespace amsi
{
  /// @brief Default constructor foraa range ProcessSet, takes low and high ranks
  /// @param l The first process rank in the set
  /// @param h One over the last process rank in the set
  ProcessSet_T<std::pair<int,int> >::ProcessSet_T(int l, int h)
    : val(std::make_pair(l,h))
  {}
  ProcessSet_T<std::pair<int,int> >::ProcessSet_T(std::pair<int,int> r)
    : val(r)
  {}
  /// @brief Constructor taking a std::pair<int,int> range instead of high and low values
  /// @param r The pair representing the range for the process set
  ProcessSet_T<std::pair<int,int> >::ProcessSet_T(std::pair<int,int> & r)
    : val(r)
  {}
  /// @brief Retrieve process ranks from the set
  /// @param i The index of the rank the retrieve
  /// @return int The i-th rank in the set
  int ProcessSet_T<std::pair<int,int> >::operator[](int i) const
  {
    return i < 0 ? -1 : i <= (val.second - val.first) ? val.first + i : -1;
  }
  /// @brief How many ranks are in the set
  /// @return int The set size
  int ProcessSet_T<std::pair<int,int> >::size() const
  {
    //std::cout << val.first << " " << val.second << std::endl;
    return val.second - val.first;
  }
  /// @brief Determine whether a rank is in the set
  /// @param p The rank to check
  /// @return bool Whether the rank is in the set or not
  bool ProcessSet_T<std::pair<int,int> >::isIn(int p) const
  {
    return p >= val.first && p < val.second;
  }
  /// @brief Extract a subset of this ProcessSet consisting of s ranks
  /// @param s How many ranks to include in the new set
  /// @return ProcessSet* The newly-created ProcessSet
  ProcessSet * ProcessSet_T<std::pair<int,int> >::extract(int s)
  {
    ProcessSet *result = nullptr;
    if(s <= size())
    {
      result =
        static_cast<ProcessSet*>(new ProcessSet_T<std::pair<int,int> >((*this)[0],(*this)[s]));
      val.first += s;
    }
    return result;
  }
  int ProcessSet_T<std::pair<int,int> >::indexOf(int r) const
  {
    int result = -1;
    if(isIn(r))
      result = r - val.first;
    return result;
  }
  // functions for std::set process set
    ProcessSet_T<std::set<int>>::ProcessSet_T(int l,int h)
    {
      valSet = std::set<int>();
      assert(l<=h);
      for(int i=l; i<h; ++i)
      {
        valSet.insert(i);
      }
      lowVal = l;
      highVal = h;
    }
    ProcessSet_T<std::set<int>>::ProcessSet_T()
    {
      valSet = std::set<int>();
      lowVal = highVal = -1;
    }
    int ProcessSet_T<std::set<int>>::operator[](int i) const
    {
      if (i>int(valSet.size()-1) || i<0)
        return -1;
      std::set<int>::iterator it = valSet.begin();
      std::advance(it, i);
      return *it;
    }
    int ProcessSet_T<std::set<int>>::size() const
    {
      return valSet.size();
    }
    bool ProcessSet_T<std::set<int>>::isIn(int i) const
    {
      std::set<int>::iterator it = valSet.find(i);
      if(it == valSet.end())
        return false;
      return true;
    }
    int ProcessSet_T<std::set<int>>::indexOf(int r) const
    {
      int result = -1;
      std::set<int>::iterator it = valSet.find(r);
      if(it == valSet.end())
        return result;
      result = std::distance(valSet.begin(), it);
      return result;
    }
    void ProcessSet_T<std::set<int>>::insert(int i)
    {
      valSet.insert(i);
    }
    ProcessSet * ProcessSet_T<std::set<int>>::extract(int size)
    {
      ProcessSet_T<std::set<int>> * result = NULL; 
      if(size <= this->size()) {
        result = new ProcessSet_T<std::set<int>>();
        std::set<int>::iterator it;
        int cnt = 0;
        for (it = valSet.begin(); it != valSet.end() && cnt < size;
             ++it, ++cnt) {
          result->insert(*it);
        }
        valSet.erase(valSet.begin(), it);
        result->setRange((*result)[0], lowVal + size);
      }
      return static_cast<ProcessSet *>(result);
    }
    ProcessSet * ProcessSet_T<std::set<int>>::extractStrided(int size, int stride, bool onStride)
    {
      ProcessSet_T<std::set<int>> * result = NULL;
      if(size/stride <= this->size())
      {
        result = new ProcessSet_T<std::set<int>>();
        std::set<int>::iterator it;
        int cnt=0;
        for(it = valSet.begin(); it!=valSet.end() && cnt<size; ++it)
        {
          int remainder = (*it % stride);
          if(remainder == 0 && onStride)
          {
            result->insert(*it);
            valSet.erase(it);
            ++cnt;
          }
          else if(remainder != 0 && !onStride)
          {
            result->insert(*it);
            valSet.erase(it);
            ++cnt;
          }
        }
        result->setRange(lowVal, highVal);
      }
      return static_cast<ProcessSet *>(result);
    }
}
