#ifndef AMSI_VERBOSITY_H_
#ifdef AMSI_VERBOSE_1
#define AMSI_V1(out) out
#else
#define AMSI_V1(out) while(false);
#endif
#ifdef AMSI_VERBOSE_2
#define AMSI_V2(out) out
#else
  // need this to compile, but compiler should optimize away
#define AMSI_V2(out) while(false);
#endif
#ifdef AMSI_VERBOSE_3
#define AMSI_V3(out) out
#else
  // need this to compile, but compiler should optimize away
#define AMSI_V3(out) while(false);
#endif
#endif
