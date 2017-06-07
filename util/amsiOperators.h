#ifndef AMSI_OPERATORS_H_
#define AMSI_OPERATORS_H_
namespace amsi
{
  struct to_R1
  {
    virtual ~to_R1() {};
    virtual double operator()() = 0;
  };
  struct to_R2
  {
    virtual ~to_R2() {};
    virtual double operator()(double &, double &) = 0;
  };
  struct to_R3
  {
    virtual ~to_R3() {};
    virtual double operator()(double &, double &, double &) = 0;
  };
  struct R1_to_R1
  {
    virtual ~R1_to_R1() {};
    virtual double operator()(double) = 0;
  };
  struct R1_to_R2
  {
    virtual ~R1_to_R2() {};
    virtual void operator()(double, double &, double &) = 0;
  };
  struct R1_to_R3
  {
    virtual ~R1_to_R3() {};
    virtual void operator()(double, double &, double &, double &) = 0;
  };
  struct R2_to_R1
  {
    virtual ~R2_to_R1() {};
    virtual double operator()(double, double) = 0;
  };
  struct R2_to_R2
  {
    virtual ~R2_to_R2() {};
    virtual void operator()(double, double, double &, double &) = 0;
  };
  struct R2_to_R3
  {
    virtual ~R2_to_R3() {};
    virtual void operator()(double, double, double &, double &, double &) = 0;
  };
  struct R3_to_R1
  {
    virtual ~R3_to_R1() {};
    virtual double operator()(double, double, double) = 0;
  };
  struct R3_to_R2
  {
    virtual ~R3_to_R2() {};
    virtual void operator()(double, double, double, double &, double &) = 0;
  };
  struct R3_to_R3
  {
    virtual ~R3_to_R3() {};
    virtual void operator()(double, double, double, double &, double &, double &) = 0;
  };
}
#endif
