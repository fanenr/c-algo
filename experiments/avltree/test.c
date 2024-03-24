#include "avltree.h"
#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 1000000UL

static void init (void);
static void clear (void);

static void test_find (void);
static void test_insert (void);
static void test_remove (void);

static int height_of (const avltree_node_t *root);
static void check_bf (const avltree_node_t *node);

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

  for (size_t i = 0; i < T; i++)
    {
      init ();

      test_insert ();
      check_bf (map.root);
      int height = height_of (map.root);
      assert (height == (map.root ? map.root->height : -1));
      printf ("len: %lu\nheight: %d\n", map.size, height);

      test_find ();

      test_remove ();
      check_bf (map.root);
      height = height_of (map.root);
      assert (height == (map.root ? map.root->height : -1));
      printf ("len: %lu\nheight: %d\n", map.size, height);

      test_find ();

      clear ();
    }
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

static inline void
test_find (void)
{
  data temp;

  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;

      temp.key = names[i];

      avltree_node_t *node = avltree_find (&map, &temp.tree_node);
      data *container = container_of (node, data, tree_node);
      assert (container->val == ages[i]);
    }
}

static inline void
test_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      char *name = rand_string (rand_long (8, 17));
      int age = rand_long (1, 101);
      assert (name);

      names[i] = name;
      ages[i] = age;
    }

  for (size_t i = 0; i < N; i++)
    {
      data *new = data_avltree_node_new (names[i], ages[i]);
      avltree_node_t *node = avltree_insert (&map, &new->tree_node);

      if (!node)
        names[i] = NULL;
    }
}

static inline void
test_remove (void)
{
  data temp;

  for (size_t i = 0; i < N; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;

      temp.key = names[rmpos];

      avltree_node_t *node = avltree_remove (&map, &temp.tree_node);
      data *container = container_of (node, data, tree_node);

      assert (!avltree_find (&map, &temp.tree_node));
      assert (container->val == ages[rmpos]);

      free (container);
      free (names[rmpos]);
      names[rmpos] = NULL;
    }
}

static inline int
height_of (const avltree_node_t *root)
{
  if (!root)
    return -1;

  int left = height_of (root->left);
  int right = height_of (root->right);
  return (left > right ? left : right) + 1;
}

static inline void
check_bf (const avltree_node_t *node)
{
  if (!node)
    return;

  check_bf (node->left);
  check_bf (node->right);

  int lh = node->left ? node->left->height : -1;
  int rh = node->right ? node->right->height : -1;
  int bf = lh - rh;

  assert (bf >= -1 && bf <= 1);
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
