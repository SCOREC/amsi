#ifndef DATADISTRIBUTION_H_
#define DATADISTRIBUTION_H_
#include "Assemblable.h"
#include "amsiMetaConfig.h"
#ifdef CORE
#include <PCU.h>
#endif
#include <valarray>
#include <vector>
#ifdef ZOLTAN
struct Zoltan_Struct;
#endif
namespace amsi
{
  class DataDistribution : public Assemblable
  {
  public:
    DataDistribution(int size);
    // Assemblable interface
    virtual void Assemble(MPI_Comm);
    int operator[](unsigned index) const;
    int& operator[](unsigned index);
    // preliminary version to get weighted migration working, HACKY and poorly implemented
    void setWeight(unsigned index, unsigned sub_index, double wgt);
    double getWeight(unsigned index, unsigned sub_index);
#   ifdef ZOLTAN
    DataDistribution(int size, Zoltan_Struct *);
    Zoltan_Struct * GetZS() const {return zs;}
#   endif
  protected:
    DataDistribution();
    bool assembled;
    bool valid;
    std::valarray<std::vector<double> > wgts;
    std::valarray<int> dd;
#   ifdef ZOLTAN
    Zoltan_Struct * zs;
#   endif
  };
} // namespace amsi

#endif
