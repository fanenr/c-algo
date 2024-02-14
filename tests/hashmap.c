#include "../hashmap.h"

long
si_hashmap_hash (char *k)
{
  return k[0];
}

int
si_hashmap_comp (char *k1, char *k2)
{
  return k1[0] == k2[0];
}

HASHMAP_DEF_INFO (char *, int, si);

int
main (void)
{
}