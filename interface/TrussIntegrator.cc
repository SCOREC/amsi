#include "TrussIntegrator.h"
#include "ConvenienceFunctions.h"
namespace amsi
{
  TrussIntegrator::TrussIntegrator(apf::Field * field,
                                   int o,
                                   double a,
                                   double b) :
    apf::Integrator(o),
    f(field),
    A(a),
    B(b)
  {
    mesh = apf::getMesh(f);
    num_field_components = apf::countComponents(f);
  }
  void TrussIntegrator::inElement(apf::MeshElement * melm)
  {
    me = melm;
    e = apf::createElement(f,me);
    num_element_nodes = apf::countNodes(e);
    int new_num_element_eqs = num_element_nodes * num_field_components;
    bool reallocate = new_num_element_eqs != num_element_eqs;
    num_element_eqs = new_num_element_eqs;
    // may assume 2-node trusses
    apf::MeshEntity * ment = apf::getMeshEntity(me);
    mesh->getPoint_(ment,0,xyz[0]);
    mesh->getPoint_(ment,1,xyz[1]);
    apf::getVectorNodes(e,node_values);
    if(reallocate)
    {
      ke.setSize(num_element_eqs,num_element_eqs);
      fe.setSize(num_element_eqs);
    }
    ke *= 0.0;
    fe.zero();
  }
  void TrussIntegrator::atPoint(apf::Vector3 const & p,
                                double w,
                                double dV)
  {
    double init_len = 0.0; // TODO: calculate initial length using coords...
    xyz[0] = xyz[0] + node_values[0];
    xyz[1] = xyz[1] + node_values[1];
    double current_len = sqrt(pow((xyz[1][0]-xyz[0][0]),2) +
                              pow((xyz[1][1]-xyz[0][1]),2) +
                              pow((xyz[1][2]-xyz[0][2]),2) );
    double stretch_ratio = current_len / init_len; //strain ...
    double fiber_force, dforce_dlength;
    FiberAxialForce(A,B,
                    stretch_ratio,
                    init_len,
                    dforce_dlength,
                    fiber_force);
    double force_per_length = fiber_force / current_len; // newtons ...
    double difference = dforce_dlength - force_per_length;
    int & n = num_field_components;
    for(int ii = 0; ii < n; ii++)
      for(int jj = 0; jj < n; jj++)
      {
        double to_add = ii == jj ? force_per_length : 0.0;
        ke(ii,jj) = difference * (
          ((xyz[0][ii] - xyz[1][ii]) + (xyz[0][jj] - xyz[1][jj]) ) / current_len
          ) + to_add;
        ke(ii+n,jj)   = -ke(ii,jj);
        ke(ii,jj+n)   = -ke(ii,jj);
        ke(ii+n,jj+n) =  ke(ii,jj);
      }
  }
}
