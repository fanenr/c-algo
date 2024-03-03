#include "avlmap.h"
#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 5UL
#define N 3000000UL

avlmap map;
int ages[N];
char *names[N];

static void clear (void);
static double bench_find (void);
static double bench_insert (void);
static double bench_remove (void);

static int si_comp (const char **a, const char **b);
AVLMAP_DEF_ALL (si, char *, int, si_comp);

int
main (void)
{
  rand_init ();

  double t_insert = 0;
  double t_remove = 0;
  double t_find = 0;

  for (size_t i = 0; i < T; i++)
    {
      t_insert += bench_insert ();
      t_remove += bench_remove ();
      t_find += bench_find ();
      clear ();
    }

  printf ("insert: %lf\n", t_insert / T);
  printf ("remove: %lf\n", t_remove / T);
  printf ("find: %lf\n", t_find / T);
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
  avlmap_free (&map);

  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
}

static inline double
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

  return TIME_VAL ();
}

static inline double
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

  return TIME_VAL ();
}

static inline double
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

  return TIME_VAL ();
}
