#include "../bitmap.h"
#include "../common.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 1000000UL

static void init (void);
static void clear (void);

static void test_fill (void);
static void test_zero (void);

static void test_set (void);
static void test_flip (void);
static void test_clear (void);

static void test_find_first0 (void);
static void test_find_first1 (void);

bitmap_t map;

int
main (void)
{
  rand_init ();

  for (size_t i = 0; i < T; i++)
    {
      init ();

      test_fill ();
      test_zero ();

      test_set ();
      test_flip ();
      test_clear ();

      test_find_first0 ();
      test_find_first1 ();

      clear ();
    }
}

static inline void
init (void)
{
  map = BITMAP_INIT;

  map.nbits = N;
  map.data = malloc (BITMAP_BYTES (N));

  assert (map.data);
}

static inline void
clear (void)
{
  free (map.data);
}

static inline void
test_fill (void)
{
  bitmap_fill (&map);

  size_t elems = BITMAP_ELEMS (map.nbits);
  for (size_t i = 0; i < elems; i++)
    assert (map.data[i] == ~0UL);
}

static inline void
test_zero (void)
{
  bitmap_zero (&map);

  size_t elems = BITMAP_ELEMS (map.nbits);
  for (size_t i = 0; i < elems; i++)
    assert (map.data[i] == 0UL);
}

static inline void
test_set (void)
{
  for (size_t i = 0; i < N; i++)
    {
      size_t pos = rand_long (0, N);
      size_t count = map.count;

      int old = bitmap_at (&map, pos);
      bitmap_set (&map, pos);

      assert (map.count == count + (old ? 0 : 1));
      assert (bitmap_at (&map, pos) == 1);
    }
}

static inline void
test_flip (void)
{
  for (size_t i = 0; i < N; i++)
    {
      size_t pos = rand_long (0, N);
      size_t count = map.count;

      int old = bitmap_at (&map, pos);
      bitmap_flip (&map, pos);

      assert (map.count == count + (old ? -1 : 1));
      assert (bitmap_at (&map, pos) == !old);
    }
}

static inline void
test_clear (void)
{
  for (size_t i = 0; i < N; i++)
    {
      size_t pos = rand_long (0, N);
      size_t count = map.count;

      int old = bitmap_at (&map, pos);
      bitmap_clear (&map, pos);

      assert (map.count == count + (old ? -1 : 0));
      assert (bitmap_at (&map, pos) == 0);
    }
}

static inline void
test_find_first0 (void)
{
  bitmap_fill (&map);

  size_t min = map.nbits;

  for (size_t i = 0; i < N; i++)
    {
      size_t pos = rand_long (0, N);
      min = pos < min ? pos : min;
      bitmap_clear (&map, pos);

      assert (bitmap_find_first0 (&map) == min);
    }
}

static inline void
test_find_first1 (void)
{
  bitmap_zero (&map);

  size_t min = map.nbits;

  for (size_t i = 0; i < N; i++)
    {
      size_t pos = rand_long (0, N);
      min = pos < min ? pos : min;
      bitmap_set (&map, pos);

      assert (bitmap_find_first1 (&map) == min);
    }
}
