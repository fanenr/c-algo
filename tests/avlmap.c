#include "../avlmap.h"
#include <string.h>

int
si_avlmap_comp (const char **key1, const char **key2)
{
  const char *s1 = *key1;
  const char *s2 = *key2;
  return strcmp (s1, s2);
}

AVLMAP_DEF_ALL (char *, int, si);
