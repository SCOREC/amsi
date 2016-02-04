#ifndef AMSIUTILCONFIG_H_IN_
#define AMSIUTILCONFIG_H_IN_
#define AMSI_MAJOR_VERSION 1
#define AMSI_MINOR_VERSION 0
#define AMSI_PATCH_VERSION 0
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)
#define SCOREC
#if TRUE == TRUE
#  define CORE
#endif
#if TRUE == TRUE
#  define SIM
#endif
#define PETSC
#ifndef NDEBUG
#  define AMSI_DEBUG(EX) EX
#else
#  define AMSI_DEBUG(EX)
#endif
#endif
