#ifndef AMSI_CONSTRAINT_H_
#define AMSI_CONSTRAINT_H_
namespace amsi
{
  class Dof
  {
  protected:
    double dof;
    int id;
    bool owned;
  public:
    virtual void setDof(double d) { dof = d; }
    virtual double getDof() { return dof; }
    virtual void setNumber(int i) { id = i; }
    virtual int getNumber() { return id; }
    virtual void setOwned(bool o) { owned = o; }
    virtual bool isOwned() { return owned; }
  };
};
#endif
