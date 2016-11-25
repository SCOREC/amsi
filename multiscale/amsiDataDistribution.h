#ifndef AMSI_DATADISTRIBUTION_H_
#define AMSI_DATADISTRIBUTION_H_
#include "amsiAssemblable.h"
#include "amsiDistributed.h"
#include "amsiMultiscaleConfig.h"
#include "amsiScale.h"
#include "amsiTask.h"
#include <PCU.h>
#include <valarray>
#include <vector>
#ifdef ZOLTAN
struct Zoltan_Struct;
#endif
namespace amsi
{
  class DistributedData : public Distributed
  {
  private:
    Scale * scl;
    int lcl_cnt;
    std::valarray<int> wgts;
  public:
    DistributedData(Scale * s,int cnt = 0);
    virtual void synchronize();
    int operator[](unsigned) const;
    int& operator[](unsigned);
    void setWeight(unsigned,double);
    double getWeight(unsigned);
  };
  class DataDistribution : public Assemblable
  {
  public:
#   ifdef ZOLTAN
    DataDistribution(int size, bool wgtd = false, Zoltan_Struct * zs = NULL);
    Zoltan_Struct * GetZS() const {return zs;}
#   else
    DataDistribution(int size, bool wgtd = false);
#   endif
    virtual ~DataDistribution() {};
    // Assemblable interface
    virtual void Assemble(MPI_Comm);
    int operator[](unsigned index) const;
    int& operator[](unsigned index);
    // preliminary version to get weighted migration working, HACKY and poorly implemented
    bool isWeighted() { return wgtd;}
    void setWeight(unsigned index, unsigned sub_index, double wgt);
    double getWeight(unsigned index, unsigned sub_index);
  protected:
    DataDistribution();
    bool valid;
    bool wgtd;
    std::valarray<std::vector<double> > wgts;
    std::valarray<int> dd;
#   ifdef ZOLTAN
    Zoltan_Struct * zs;
#   endif
  };
} // namespace amsi
#endif
