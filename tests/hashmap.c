#include "../hashmap.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define N 100000

char *names[N];
int ages[N];

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

HASHMAP_DEF_ALL (char *, int, si);

int
main (void)
{
  hashmap map;
  hashmap_init (&map);

  for (size_t i = 0; i < N; i++)
    {
      char *name = rand_string (rand_long (8, 17));
      assert (name);
      int age = rand_long (1, 101);
      names[i] = name;
      ages[i] = age;
      si_hashmap_n *node = si_hashmap_insert (&map, name, age);
      if (!node)
        names[i] = NULL;
    }

  /* printf ("len: %lu\ncap: %lu\n", map.len, map.cap); */

  for (size_t i = 0; i < N / 4; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;
      size_t len = map.len;
      si_hashmap_remove (&map, names[rmpos]);
      assert (map.len != len);
      free (names[rmpos]);
      names[rmpos] = NULL;
    }

  /* printf ("len: %lu\n", map.len); */

  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;
      si_hashmap_n *node = si_hashmap_find (&map, names[i]);
      assert (node);
      assert (node->val == ages[i]);
    }

  for (size_t i = 0; i < N; i++)
    free (names[i]);
  hashmap_free (&map);
}
