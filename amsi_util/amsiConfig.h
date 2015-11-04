#ifndef AMSICONFIG_H_IN_
#define AMSICONFIG_H_IN_

#define AMSI_MAJOR_VERSION 1
#define AMSI_MINOR_VERSION 0
#define AMSI_PATCH_VERSION 0

#define HOST SCOREC

#if TRUE == TRUE
#  define CORE 1
#else
#  define CORE 0
#endif

#if TRUE == TRUE
#  define SIM 1
#else
#  define SIM 0
#endif

#if TRUE == TRUE
#  define ZOLTAN 1
#else
#  define ZOLTAN 0
#endif

#if TRUE == TRUE
#  define PETSC 1
#else
#  define PETSC 0
#endif

#ifdef DEBUG
#  define AMSI_DEBUG(EX) EX
#else 
#  define AMSI_DEBUG(EX)
#endif

#endif
