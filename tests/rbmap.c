#include "../rbmap.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 1000000UL

rbmap map;
int ages[N];
char *names[N];

static void clear (void);
static void test_find (void);
static void test_insert (void);
/* static void test_remove (void); */

static int si_comp (const char **a, const char **b);
RBMAP_DEF_ALL (si, char *, int, si_comp);

int
main (void)
{
  for (size_t i = 0; i < T; i++)
    {
      test_insert ();
      printf ("len: %lu\n", map.size);

      /*
      test_remove ();
      printf ("len: %lu\n", map.size);
      */

      test_find ();
      clear ();
    }
}

static inline int
si_comp (const char **a, const char **b)
{
  return strcmp (*a, *b);
}

static inline void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  rbmap_free (&map);

  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
}

static inline void
test_find (void)
{
  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;

      si_rbmap_n *node = si_rbmap_find (&map, names[i]);
      assert (node->val == ages[i]);
    }
}

static inline void
test_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      char *name = rand_string (rand_long (8, 17));
      assert (name);
      int age = rand_long (1, 101);
      names[i] = name;
      ages[i] = age;
    }

  for (size_t i = 0; i < N; i++)
    {
      si_rbmap_n *node = si_rbmap_insert (&map, names[i], ages[i]);
      if (!node)
        names[i] = NULL;
    }
}

/*
static inline void
test_remove (void)
{
  for (size_t i = 0; i < N / 2; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;

      size_t size = map.size;
      si_rbmap_remove (&map, names[rmpos]);
      assert (map.size != size);

      free (names[rmpos]);
      names[rmpos] = NULL;
    }
}
*/
