#ifndef AMSI_CONVERSION_ITERATOR_H_
#define AMSI_CONVERSION_ITERATOR_H_
namespace amsi
{
  template <typename T, class OP, typename W>
    class ConversionIterator
  {
  private:
    W wrpd;
    OP op;
  public:
  ConversionIterator(W w, OP o)
      : wrpd(w)
      , op(o)
    { }
    T operator*()
    {
      return op(*wrpd);
    }
    ConversionIterator & operator++()
    {
      wrpd++;
      return *this;
    }
    bool operator==(ConversionIterator & o)
    {
      return o.wrpd == wrpd && o.op == op;
    }
    bool operator!=(ConversionIterator & o)
    {
      return !(*this).operator==(o);
    }
  };
}
#endif
