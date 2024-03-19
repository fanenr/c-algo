#include "../vector.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 1000000UL

static void clear (void);
static void test_at (void);
static void test_insert (void);
static void test_remove (void);

VECTOR_DEF_ALL (age, int);
VECTOR_DEF_ALL (name, char *);

int ages[N];
char *names[N];
age_vector avec;
name_vector nvec;

int
main (void)
{
  rand_init ();

  for (size_t i = 0; i < T; i++)
    {
      test_insert ();
      printf ("size: %lu, %lu\n", avec.size, nvec.size);

      test_remove ();
      printf ("size: %lu, %lu\n", avec.size, nvec.size);

      test_at ();
      clear ();
    }
}

static inline void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  age_vector_free (&avec);
  name_vector_free (&nvec);

  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
}

static inline void
test_at (void)
{
  for (size_t i = 0; i < N; i++)
    {
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
    }
}

static inline void
test_remove (void)
{
  for (size_t i = 0; i < N; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;
    }
}
