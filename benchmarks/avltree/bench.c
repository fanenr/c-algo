#include "avltree.h"
#include "common.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 10000000UL

static void init (void);
static void clear (void);

static double bench_find (void);
static double bench_insert (void);
static double bench_remove (void);

typedef struct data data;

struct data
{
  avltree_node_t tree_node;
  char *key;
  int val;
};

int ages[N];
avltree_t map;
char *names[N];

#define data_avltree_node_new(data_key, data_val)                             \
  ({                                                                          \
    data *new = malloc (sizeof (data));                                       \
    new->tree_node = avltree_node_init;                                       \
    new->key = (data_key);                                                    \
    new->val = (data_val);                                                    \
    new;                                                                      \
  })

static void data_avltree_free (avltree_t *tree);

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
      t_remove += bench_remove ();
      t_find += bench_find ();
      clear ();
    }

  printf ("insert: %lf\n", t_insert / T);
  printf ("remove: %lf\n", t_remove / T);
  printf ("find: %lf\n", t_find / T);
}

static inline int
data_comp (const avltree_node_t *a, const avltree_node_t *b)
{
  const data *da = container_of (a, data, tree_node);
  const data *db = container_of (b, data, tree_node);
  return strcmp (da->key, db->key);
}

static void
init (void)
{
  avltree_init (&map, data_comp);
}

static inline void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  data_avltree_free (&map);

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

      data temp = { .key = names[i] };
      avltree_node_t *node = avltree_find (&map, &temp.tree_node);
      data *container = container_of (node, data, tree_node);

      if (container->val != ages[i])
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
      int age = rand_long (1, 101);
      assert (name);

      names[i] = name;
      ages[i] = age;
    }

  TIME_ST ();
  for (size_t i = 0; i < N; i++)
    {
      data *new = data_avltree_node_new (names[i], ages[i]);
      avltree_node_t *node = avltree_insert (&map, &new->tree_node);
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

      data temp = { .key = names[rmpos] };
      avltree_node_t *node = avltree_remove (&map, &temp.tree_node);
      data *container = container_of (node, data, tree_node);

      names[rmpos] = NULL;
      free (container);
    }
  TIME_ED ();

  return TIME_VAL ();
}

static void
data_avltree_free_impl (avltree_node_t *node)
{
  if (!node)
    return;

  data_avltree_free_impl (node->left);
  data_avltree_free_impl (node->right);
  free (container_of (node, data, tree_node));
}

static void
data_avltree_free (avltree_t *tree)
{
  data_avltree_free_impl (tree->root);
  avltree_init (tree, tree->comp_fn);
}
