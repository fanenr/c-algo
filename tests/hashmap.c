#include "../hashmap.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define N 100000

char *names[N];
int ages[N];

hashmap map;

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

static void
init (void)
{
  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
  hashmap_init (&map);
}

static void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  hashmap_free (&map);
}

static void
test_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      char *name = NULL;
      assert ((name = rand_string (rand_long (8, 17))));
      int age = rand_long (1, 101);
      names[i] = name;
      ages[i] = age;
    }

  for (size_t i = 0; i < N; i++)
    {
      si_hashmap_n *node = si_hashmap_insert (&map, names[i], ages[i]);
      if (!node)
        names[i] = NULL;
    }
}

static void
test_remove (void)
{
  for (size_t i = 0; i < N / 2; i++)
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
}

static void
test_find (void)
{
  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;

      si_hashmap_n *node = si_hashmap_find (&map, names[i]);
      assert (node->val == ages[i]);
    }
}

int
main (void)
{
  size_t times = 1;
  for (size_t i = 0; i < times; i++)
    {
      init ();

      test_insert ();
      printf ("len: %lu\ncap: %lu\n", map.len, map.cap);

      test_remove ();
      printf ("len: %lu\n", map.len);

      test_find ();

      clear ();
    }
}
