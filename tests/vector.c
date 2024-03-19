#include "../vector.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define T 1UL
#define N 100000UL

static void init (void);
static void clear (void);

static void test_at (void);
static void test_insert (void);
static void test_remove (void);

vector_t vec;

int
main (void)
{
  rand_init ();

  for (size_t i = 0; i < T; i++)
    {
      init ();

      test_insert ();
      printf ("size: %lu\n", vec.size);

      test_remove ();
      printf ("size: %lu\n", vec.size);

      test_at ();

      clear ();
    }
}

static inline void
init (void)
{
  vector_init (&vec, sizeof (int), NULL);
}

static inline void
clear (void)
{
  vector_free (&vec);
}

static inline void
test_at (void)
{
  for (size_t i = 0; i < vec.size; i++)
    {
      int *node = vector_at (&vec, i);
      assert (node);
    }
}

static inline void
test_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      size_t pos = rand_long (0, vec.size + 1);
      int *node = vector_insert (&vec, pos);
      int num = rand_long (0, N);
      *node = num;
    }
}

static inline void
test_remove (void)
{
  for (size_t i = 0; i < N / 2; i++)
    {
      size_t pos = rand_long (0, vec.size);
      vector_remove (&vec, pos);
    }
}
