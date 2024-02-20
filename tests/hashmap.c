#include "../hashmap.h"
#include <assert.h>
#include <string.h>

size_t
ii_hashmap_hash (const int *key)
{
  return *key;
}

int
ii_hashmap_comp (const int *key1, const int *key2)
{
  int i1 = *key1;
  int i2 = *key2;
  if (i1 < i2)
    return -1;
  if (i1 > i2)
    return 1;
  return 0;
}

size_t
si_hashmap_hash (const char **key)
{
  const char *str = *key;
  size_t hash = 0;
  for (size_t i = strlen (str); i; i--)
    hash += str[i - 1];
  return hash;
}

int
si_hashmap_comp (const char **key1, const char **key2)
{
  const char *s1 = *key1;
  const char *s2 = *key2;
  return strcmp (s1, s2);
}

HASHMAP_DEF_ALL (int, int, ii);
HASHMAP_DEF_ALL (char *, int, si);

int
main (void)
{
  hashmap map;
  hashmap_init (&map);

  si_hashamp_insert (&map, "Tom", 18);
  si_hashamp_insert (&map, "Jack", 19);
  si_hashamp_insert (&map, "Arthur", 20);

  assert (si_hashamp_find (&map, "Tom")->val == 18);
  assert (si_hashamp_find (&map, "Jack")->val == 19);
  assert (si_hashamp_find (&map, "Arthur")->val == 20);

  hashmap_free (&map);
}
