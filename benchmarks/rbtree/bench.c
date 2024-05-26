#include "common.h"
#include "rbtree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 3UL
#define N 1000000UL

static void init (void);
static void clear (void);

static double bench_find (void);
static double bench_insert (void);
static double bench_remove (void);

typedef struct data data;

struct data
{
  rbtree_node_t tree_node;
  char *key;
  int val;
};

int ages[N];
rbtree_t map;
char *names[N];

#define data_rbtree_node_new(data_key, data_val)                              \
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
  init ();

  double t_insert = 0;
  double t_remove = 0;
  double t_find = 0;

  for (size_t i = 0; i < T; i++)
    {
      t_insert += bench_insert ();
      t_find += bench_find ();
      t_remove += bench_remove ();
      t_find += bench_find ();
      clear ();
    }

  printf ("insert: %lf\n", t_insert / T);
  printf ("remove: %lf\n", t_remove / T);
  printf ("find: %lf\n", t_find / T);
}

static inline int
comp (const rbtree_node_t *a, const rbtree_node_t *b)
{
  const data *da = container_of (a, data, tree_node);
  const data *db = container_of (b, data, tree_node);
  return strcmp (da->key, db->key);
}

static inline void
dtor (rbtree_node_t *n)
{
  data *d = container_of (n, data, tree_node);
  free (d);
}

static void
init (void)
{
  map = RBTREE_INIT;
}

static inline void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  rbtree_for_each (&map, dtor);
  map = RBTREE_INIT;

  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
}

static inline data *
data_insert (rbtree_t *tree, data *node)
{
  rbtree_node_t *parent = NULL;
  rbtree_node_t **inpos = &tree->root;
  rbtree_node_t *tree_node = &node->tree_node;

  for (rbtree_node_t *curr = tree->root; curr;)
    {
      int comp_ret = comp (tree_node, curr);

      if (comp_ret != 0)
        {
          parent = curr;
          inpos = comp_ret < 0 ? &curr->left : &curr->right;
          curr = *inpos;
          continue;
        }

      return NULL;
    }

  rbtree_link (tree, inpos, parent, &node->tree_node);

  return node;
}

static inline data *
data_find (rbtree_t *tree, const data *target)
{
  const rbtree_node_t *tree_node = &target->tree_node;

  for (rbtree_node_t *curr = tree->root; curr;)
    {
      int comp_ret = comp (tree_node, curr);

      if (comp_ret == 0)
        return container_of (curr, data, tree_node);

      curr = comp_ret < 0 ? curr->left : curr->right;
    }

  return NULL;
}

static inline double
bench_find (void)
{
  TIME_ST ();
  data temp;
  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;

      temp.key = names[i];

      data *node = data_find (&map, &temp);
      assert (node->val == ages[i]);
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
      int age = rand_long (1, 101);
      assert (name);

      names[i] = name;
      ages[i] = age;
    }

  TIME_ST ();
  for (size_t i = 0; i < N; i++)
    {
      data *new = data_rbtree_node_new (names[i], ages[i]);
      data *node = data_insert (&map, new);

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
  data temp;
  for (size_t i = 0; i < N / 2; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;

      temp.key = names[rmpos];

      data *node = data_find (&map, &temp);
      rbtree_erase (&map, &node->tree_node);
      free (node);

      names[rmpos] = NULL;
    }
  TIME_ED ();

  return TIME_VAL ();
}
