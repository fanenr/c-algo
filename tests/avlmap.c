#include "../avlmap.h"
#include <assert.h>
#include <string.h>

int
si_avlmap_comp (const char **key1, const char **key2)
{
  const char *s1 = *key1;
  const char *s2 = *key2;
  return strcmp (s1, s2);
}

AVLMAP_DEF_ALL (char *, int, si);

int
main (void)
{
  avlmap map;
  avlmap_init (&map);

  si_avlmap_insert (&map, "Tom", 18);
  si_avlmap_insert (&map, "Jack", 19);
  si_avlmap_insert (&map, "Arthur", 20);

  assert (si_avlmap_find (&map, "Tom")->val == 18);
  assert (si_avlmap_find (&map, "Jack")->val == 19);
  assert (si_avlmap_find (&map, "Arthur")->val == 20);

  avlmap_free (&map);
}
