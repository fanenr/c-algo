#include "../list.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define T 1UL
#define N 100000UL

static void init (void);
static void clear (void);

static void test_at (void);
static void test_insert (void);
static void test_remove (void);

typedef struct data data;

struct data
{
  list_node_t list_node;
  int num;
};

list_t list;

#define data_list_node_new(data_num)                                          \
  ({                                                                          \
    data *new = malloc (sizeof (data));                                       \
    new->num = (data_num);                                                    \
    new;                                                                      \
  })

static void data_list_free (list_t *list);

int
main (void)
{
  rand_init ();

  for (size_t i = 0; i < T; i++)
    {
      init ();

      test_insert ();
      printf ("size: %lu\n", list.size);

      test_remove ();
      printf ("size: %lu\n", list.size);

      test_at ();

      clear ();
    }
}

static inline void
init (void)
{
  list_init (&list, NULL);
}

static inline void
clear (void)
{
  data_list_free (&list);
}

static inline void
test_at (void)
{
  for (size_t i = 0; i < list.size; i++)
    {
      list_node_t *node = list_at (&list, i);
      assert (node);
    }
}

static inline void
test_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      size_t index = rand_long (0, list.size + 1);
      list_node_t *pos = list_at (&list, index);
      int num = rand_long (0, N);

      data *new = data_list_node_new (num);
      list_node_t *node = list_insert_at (&list, index, &new->list_node);
      assert (node == &new->list_node);
    }
}

static inline void
test_remove (void)
{
  for (size_t i = 0; i < N / 2; i++)
    {
      size_t index = rand_long (0, list.size);
      list_node_t *node = list_at (&list, index);
      data *container = container_of (node, data, list_node);
      list_remove (&list, node);
      free (container);
    }
}

static inline void
data_list_free (list_t *list)
{
  list_node_t *curr, *next;
  for (curr = list->head; curr;)
    {
      data *container = container_of (curr, data, list_node);
      next = curr->next;
      free (container);
      curr = next;
    }
  list_init (list, list->comp_fn);
}
