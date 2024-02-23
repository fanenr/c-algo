#include "avlmap.h"
#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10000000

char *names[N];
int ages[N];

int
si_avlmap_comp (const char **key1, const char **key2)
{
  const char *s1 = *key1;
  const char *s2 = *key2;
  return strcmp (s1, s2);
}

AVLMAP_DEF_ALL (char *, int, si);

avlmap map;

static void
bench_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      char *name = rand_string (rand_long (8, 17));
      assert (name);
      int age = rand_long (1, 101);
      names[i] = name;
      ages[i] = age;
    }

  TIME_ST ();
  for (size_t i = 0; i < N; i++)
    {
      si_avlmap_n *node = si_avlmap_insert (&map, names[i], ages[i]);
      if (!node)
        names[i] = NULL;
    }
  TIME_ED ();

  printf ("insert: %lf\n", TIME_VAL ());
}

static void
bench_remove (void)
{
  TIME_ST ();
  for (size_t i = 0; i < N / 2; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;
      si_avlmap_remove (&map, names[rmpos]);
      names[rmpos] = NULL;
    }
  TIME_ED ();

  printf ("remove: %lf\n", TIME_VAL ());
}

static void
bench_find (void)
{
  TIME_ST ();
  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;
      si_avlmap_n *node = si_avlmap_find (&map, names[i]);
      if (node->val != ages[i])
        {
          printf ("find failed\n");
          abort ();
        }
    }
  TIME_ED ();

  printf ("find: %lf\n", TIME_VAL ());
}

int
main (void)
{
  bench_insert ();
  bench_remove ();
  bench_find ();
}
