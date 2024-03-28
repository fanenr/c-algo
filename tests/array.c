#include "../array.h"
#include "../array_ext.h"
#include "../common.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 100000UL

static void init (void);
static void clear (void);

static void test_at (void);
static void test_insert (void);
static void test_remove (void);

typedef struct data
{
  char *key;
  int val;
} data;

array_t arr;

int
main (void)
{
  rand_init ();

  for (size_t i = 0; i < T; i++)
    {
      init ();

      test_insert ();
      printf ("size: %lu\n", arr.size);

      test_at ();

      test_remove ();
      printf ("size: %lu\n", arr.size);

      test_at ();

      clear ();
    }
}

static inline int
comp (const void *a, const void *b)
{
  const data *da = a;
  const data *db = b;
  return strcmp (da->key, db->key);
}

static inline void
dtor (void *e)
{
  data *de = e;
  free (de->key);
}

static inline void
init (void)
{
  arr = ARRAY_INIT;
  arr.elem_size = sizeof (data);
}

static inline void
clear (void)
{
  array_for_each (&arr, dtor);
  free (arr.data);
}

static inline void
test_at (void)
{
  for (size_t i = 0; i < arr.size; i++)
    {
      data *elem = array_at (&arr, i);
      assert (elem->key);
    }
}

static inline data *
arr_insert (array_t *arr, size_t pos, const data *elem)
{
#define ARR_INIT_CAP 8
#define ARR_EXPAN_RATIO 2

  if (arr->size >= arr->cap)
    {
      size_t newcap = arr->cap;
      newcap = newcap ? newcap * ARR_EXPAN_RATIO : ARR_INIT_CAP;
      void *newdata = realloc (arr->data, newcap * arr->elem_size);
      arr->data = newdata;
      arr->cap = newcap;
    }

#undef ARR_EXPAN_RATIO
#undef ARR_INIT_CAP

  data *in = array_insert (arr, pos);
  *in = *elem;
  return in;
}

static inline void
test_insert (void)
{
  data temp;
  for (size_t i = 0; i < N; i++)
    {
      char *key = rand_string (rand_long (8, 17));
      size_t pos = rand_long (0, arr.size + 1);
      int val = rand_long (0, 101);
      assert (key);

      temp.key = key;
      temp.val = val;

      data *elem = arr_insert (&arr, pos, &temp);
    }
}

static inline void
test_remove (void)
{
  for (size_t i = 0; i < N / 2; i++)
    {
      size_t pos = rand_long (0, arr.size);
      dtor (array_at (&arr, pos));
      array_erase (&arr, pos);
    }
}
