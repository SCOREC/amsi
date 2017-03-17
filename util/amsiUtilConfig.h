#ifndef AMSI_UTILCONFIG_H_IN_
#define AMSI_UTILCONFIG_H_IN_
#define AMSI_MAJOR_VERSION 1
#define AMSI_MINOR_VERSION 0
#define AMSI_PATCH_VERSION 0
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)
#define BGQ
#define SCOREC
#define SIM
#define PETSC
#ifndef NDEBUG
#  define AMSI_DEBUG(EX) EX
#else
#  define AMSI_DEBUG(EX)
#endif
#endif
