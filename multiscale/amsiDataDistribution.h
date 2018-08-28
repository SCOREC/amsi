#ifndef AMSI_DATADISTRIBUTION_H_
#define AMSI_DATADISTRIBUTION_H_
#include "amsiAssemblable.h"
#include "amsiDistributed.h"
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
  // unused
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
    /**
     * Construct a data distribution object.
     */
    DataDistribution(int sz,
                     int l,
                     bool wgtd = false,
                     Zoltan_Struct * zs = NULL);
    /**
     * Get the zoltan structure related to this
     *  DataDistribution. Used primarily by
     *  AMSI load balancing algorithms.
     */
    Zoltan_Struct * GetZS() const
    {
      return zs;
    }
#   else
    /**
     * Construct a data distribution object.
     */
    DataDistribution(int sz,
                     int l,
                     bool wgtd = false);
#   endif
    virtual ~DataDistribution() {};
    /**
     * Assemble the DataDistribution, so that
     *  each process has full information on
     *  the current state of the parallel
     *  distribution of the modeled data.
     */
    virtual void Assemble(MPI_Comm);
    /**
     * Get the data quantity for the process with rank
     *  idx in the MPI_Comm this DataDistribution is
     *  related to. Strictly a RHS operator.
     */
    int operator[](unsigned idx) const;
    /**
     * Set the data quantity for the local process.
     */
    int operator=(int qnt);
    /**
     * Accumulate the data quantity for the local process.
     */
    int operator+=(int qnt);
    /**
     * Determine if the units of data modeled by this
     *  DataDistribution have a weighting metric associated
     *  with them.
     */
    bool isWeighted() { return wgtd;}
    /**
     * Get/Set the weight associated with the wgt-th
     *  unit of data on the n-th process.
     */
    double & getWeight(unsigned nth, unsigned wgt);
  protected:
    DataDistribution();
    int sz;
    int lcl_rnk;
    bool valid;
    bool wgtd;
    std::vector<std::vector<double>> wgts;
    std::vector<int> dd;
#   ifdef ZOLTAN
    Zoltan_Struct * zs;
#   endif
  };
} // namespace amsi
#endif
