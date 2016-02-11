#include "simWrapper.h"
namespace amsi
{
  const char * modelItemTypeString(int tp)
  {
    switch(tp)
    {
    case Gvertex:
      return "Gvertex";
    case Gedge:
      return "Gedge";
    case Gface:
      return "Gface";
    case Gregion:
      return "Gregion";
    default:
      return "unhandled type!";
    }
  }
  int modelItemTypeDim(int tp)
  {
    switch(tp)
    {
    case Gvertex:
      return 0;
    case Gedge:
      return 1;
    case Gface:
      return 2;
    case Gregion:
      return 3;
    }
  }
}
