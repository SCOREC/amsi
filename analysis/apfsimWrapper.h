#ifndef APF_SIM_WRAPPER_H_
#define APF_SIM_WRAPPER_H_
#include "sim.h"
#include <apf.h>
#include <apfMesh.h>
#include <list>
namespace amsi
{
  /*
   * \brief applies a integer tag to each region
   * \param mdl the simmetrix geometric model
   * \param msh the parallel simmetrix mesh
   * \param apfmsh pointer to the apf mesh
   * Reads the region tags from the simmetrix mesh and puts
   * them into the region_id field in the apf mesh
   */
  void applyUniqueRegionTags(pGModel mdl, pMesh msh, apf::Mesh * apfmsh);
}
#endif
