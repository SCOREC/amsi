#ifndef ERRORESTIMATORS_H_
#define ERRORESTIMATORS_H_

namespace amsi {
  namespace Analysis {

    template <class T>
    class NodalAverage : public FieldOp
    {
    private:
      apf::Mesh * mesh;
      apf::Field * ip_field;
      apf::Field * recovered_field;
    public:
    NodalAverage(apf::FieldOf<T> * in_ip_field, apf::FieldOf<T> * in_recovered_field) :
      FieldOp(),
	mesh(apf::getMesh(in_ip_field)),
	ip_field(in_ip_field),
	recovered_field(in_recovered_field)
      {
	mesh = apf::getMesh(ip_field);
      }
    };

    template <>
      class NodalAverage<apf::Matrix3x3> : public FieldOp
    {
    public:
      
      bool inEntity(apf::MeshEntity * me)
      {
	apf::Matrix3x3 recovered_value;
	apf::Adjacent elements;
	mesh->getAdjacent(me,mesh->getDimension(),elements);
	int num_adjacent = elements.getSize();
	APF_ITERATE(apf::Adjacent,elements,eit)
	{
	  apf::Matrix3x3 element_value;
	  apf::Element * e = apf::createElement(ip_field,*eit);
	  apf::NewArray<apf::Matrix3x3> fe_values;
	  apf::getMatrixNodes(e,fe_values);
	  int n = apf::countNodes(e);
	  for(int ii = 0; ii < n; ii++)
	    element_value = element_value + fe_values[ii];
	  element_value = element_value / n;
	  recovered_value = recovered_value + element_value;
	}
	recovered_value = recovered_value  / num_adjacent;
	apf::setMatrix(recovered_value,me,0,recovered_field);
      }

      void outEntity()
      { }

      void atNode(int node)
      { }
    }

    
  }
}


#endif
