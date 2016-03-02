#include "amsiHost.h"
#include <cstdlib>
int is_big_endian()
{
  union {
      unsigned int i;
      char c[4];
  } bint = {0x01020304};
  return bint.c[0] == 1;
}
