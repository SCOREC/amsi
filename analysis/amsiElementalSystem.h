#ifndef AMSI_ELEMENTAL_SYSTEM_H_
#define AMSI_ELEMENTAL_SYSTEM_H_
namespace amsi
{
  class ElementalSystem
  {
  protected:
    int nedofs;
  public:
    ElementalSystem()
      : nedofs(0)
    { }
    virtual int getNedofs() { return nedofs; }
    virtual const double * getKe() = 0;
    virtual const double * getFe() = 0;
  };
}
#endif
