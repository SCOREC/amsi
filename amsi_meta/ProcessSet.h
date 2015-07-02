#ifndef PROCESSSET_H_
#define PROCESSSET_H_

#include <utility>

namespace amsi {

    /// The interface specification to which all ProcessSet classes must adhere.
    /// A ProcessSet simply represents a mathematical set of process ranks, and allows
    ///  several operations on them.
    class ProcessSet{
    public: 
      virtual int operator[](int) = 0;
      virtual int size() = 0;
      virtual bool isIn(int) = 0;
      virtual int indexOf(int) = 0;
      virtual ProcessSet * extract(int) = 0;
    };

    /// A templated subclass of the ProcessSet abstract interface, used primarily 
    ///  to limit inheritance polymorphism 
    template <typename T>
    class ProcessSet_T : public ProcessSet
    {
    public:
      ProcessSet_T(T&);
      virtual int operator[](int);
      virtual int size();
      virtual bool isIn(int);
      virtual int indexOf(int);
      virtual ProcessSet * extract(int);
    private:
      T val;
    };

    /// A partial template specialization of the templated subclass which uses
    ///  a pair<T,T> as the primary set specifier to specify sets representing
    ///  ranges of process ranks.
    template <>
    class ProcessSet_T<std::pair<int,int> > : public ProcessSet
    {
    public:
      ProcessSet_T(int,int);
      ProcessSet_T(std::pair<int,int>);
      ProcessSet_T(std::pair<int,int> &);

      virtual int operator[](int);
      virtual int size();
      virtual bool isIn(int);
      virtual int indexOf(int);
      virtual ProcessSet * extract(int); // 'carves' off a chunk of the set
    private:
      std::pair<int,int> val;
    };
   
} // namespace amsi

#endif
