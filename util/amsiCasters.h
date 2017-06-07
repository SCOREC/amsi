#ifndef AMSI_CASTERS_H_
#define AMSI_CASTERS_H_
namespace amsi
{
  template <typename From, typename To>
  struct static_caster
  {
    To operator()(From p) {return static_cast<To>(p);}
  };
  template <typename From, typename To>
  struct dynamic_caster
  {
    To operator()(From p) {return dynamic_cast<To>(p);}
  };
  template <typename From, typename To>
  struct reinterpret_caster
  {
    To operator()(From p) {return reinterpret_cast<To>(p);}
  };
}
#endif
