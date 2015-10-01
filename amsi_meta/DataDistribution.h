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

namespace amsi {

    // May be best to implement a class for this eventually, but ATM this is all that is needed
    //typedef std::valarray<int> DataDistribution;

    class DataDistribution : public Assemblable
    {
    public:
      DataDistribution();
      DataDistribution(int size);

      void Init();

      // Assemblable interface
      int Assemble(MPI_Comm,int);
      bool isAssembled() const {return assembled;}
      
      int operator[](int index) const;
      int& operator[](int index);

      // preliminary version to get weighted migration working, HACKY and poorly implemented
      void setWeight(int index, int sub_index, double wgt);
      double getWeight(int index, int sub_index);
      
#     ifdef ZOLTAN
      DataDistribution(int size, Zoltan_Struct *);
      Zoltan_Struct * GetZS() const {return zs;}
#     endif

    private:
      bool assembled;
      bool valid;

      std::valarray<std::vector<double> > wgts;
      std::valarray<int> dd;

#     ifdef ZOLTAN
      Zoltan_Struct * zs;
#     endif
    };


} // namespace amsi

#endif
