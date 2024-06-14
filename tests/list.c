#include "../list.h"
#include "../common.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 10000UL

static void init (void);
static void clear (void);

static void test_at (void);
static void test_insert (void);
static void test_remove (void);

typedef struct data data;

struct data
{
  list_node_t list_node;
  char *key;
  int val;
};

list_t list;

#define data_node_new(data_key, data_val)                                     \
  ({                                                                          \
    data *new = malloc (sizeof (data));                                       \
    new->key = (data_key);                                                    \
    new->val = (data_val);                                                    \
    new;                                                                      \
  })

int
main (void)
{
  rand_init ();

  for (size_t i = 0; i < T; i++)
    {
      init ();

      test_insert ();
      printf ("size: %lu\n", list.size);

      test_at ();

      test_remove ();
      printf ("size: %lu\n", list.size);

      test_at ();

      clear ();
    }
}

static inline int
comp (const list_node_t *a, const list_node_t *b)
{
  const data *da = container_of (a, data, list_node);
  const data *db = container_of (b, data, list_node);
  return strcmp (da->key, db->key);
}

static inline void
dtor (list_node_t *n)
{
  data *d = container_of (n, data, list_node);
  free (d->key);
  free (d);
}

static inline void
init (void)
{
  list = LIST_INIT;
}

static inline void
clear (void)
{
  list_visit (&list, dtor);
}

static inline void
test_at (void)
{
  for (size_t i = 0; i < list.size; i++)
    {
      list_node_t *node = list_at (&list, i);
      data *d = container_of (node, data, list_node);
      assert (d->key);
    }
}

static inline void
test_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      char *key = rand_string (rand_long (8, 17));
      size_t index = rand_long (0, list.size + 1);
      int val = rand_long (0, 101);
      assert (key);

      data *new = data_node_new (key, val);
      list_insert_at (&list, index, &new->list_node);
    }
}

static inline void
test_remove (void)
{
  for (size_t i = 0; i < N / 2; i++)
    {
      size_t index = rand_long (0, list.size);
      list_node_t *node = list_at (&list, index);
      list_erase (&list, node);
      dtor (node);
    }
}
