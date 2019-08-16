#ifndef AMSI_PROCESSSET_H_
#define AMSI_PROCESSSET_H_
#include <utility>
#include <set>
namespace amsi
{
  /// The interface specification to which all ProcessSet classes must adhere.
  /// A ProcessSet simply represents a mathematical set of process ranks, and allows
  ///  several operations on them.
  class ProcessSet
  {
  public:
    virtual ~ProcessSet() { };
    virtual int operator[](int) const = 0;
    virtual int size() const = 0;
    virtual bool isIn(int) const = 0;
    virtual int indexOf(int) const= 0;
    virtual ProcessSet * extract(int) = 0;
  };
  /// A templated subclass of the ProcessSet abstract interface, used primarily
  ///  to limit inheritance polymorphism
  template <typename T>
    class ProcessSet_T : public ProcessSet
  {
  public:
    ProcessSet_T(T&);
    virtual int operator[](int) const override;
    virtual int size() const override;
    virtual bool isIn(int) const override;
    virtual int indexOf(int) const override;
    virtual ProcessSet * extract(int) override;
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
    virtual int operator[](int) const final;
    virtual int size() const final;
    virtual bool isIn(int) const final;
    virtual int indexOf(int) const final;
    virtual ProcessSet * extract(int) final; // 'carves' off a chunk of the set
  private:
    std::pair<int,int> val;
  };
  template <>
    class ProcessSet_T<std::set<int>> : public ProcessSet
  {
  private:
    typedef std::set<int> ValSet;
    ValSet valSet;
    int lowVal, highVal;
  public:
    ProcessSet_T(int,int);
    ProcessSet_T();
    virtual int operator[](int) const final;
    virtual int size() const final;
    virtual bool isIn(int) const final;
    virtual int indexOf(int) const final;
    virtual ProcessSet * extract(int) final; // 'carves' off a chunk of the set
    // extract strided values from the set. If onStride is true then it will
    // return values on the stride, otherwise it will return off stride values.
    // e.g. if stride is 4, onStride will return 0,4,8,... and offStride will return
    // 1,2,3,5,6,7,9,...
    ProcessSet * extractStrided(int size, int stride, bool onStride);
    void insert(int);
    typedef ValSet::iterator iterator;
    typedef ValSet::const_iterator const_iterator;
    iterator begin() { return valSet.begin(); }
    iterator end() { return valSet.end(); }
    void setRange(int low, int high) {lowVal=low; highVal=high; }
  };
  inline void toArray(ProcessSet * ps, int * arr)
  {
    for(int ii = 0; ii < ps->size(); ii++)
      arr[ii] = (*ps)[ii];
  }

} // namespace amsi
#endif
