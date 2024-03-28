#include "../avltree.h"
#include "../avltree_ext.h"
#include "../common.h"

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
      check_bf (map.root);
      int height = height_of (map.root);
      assert (height == (map.root ? map.root->height : -1));
      printf ("size: %lu\nheight: %d\n", map.size, height);

      test_find ();

      test_remove ();
      check_bf (map.root);
      height = height_of (map.root);
      assert (height == (map.root ? map.root->height : -1));
      printf ("size: %lu\nheight: %d\n", map.size, height);

      test_find ();

      clear ();
    }
}

static inline int
comp (const avltree_node_t *a, const avltree_node_t *b)
{
  const data *da = container_of (a, data, tree_node);
  const data *db = container_of (b, data, tree_node);
  return strcmp (da->key, db->key);
}

static inline void
dtor (avltree_node_t *n)
{
  data *d = container_of (n, data, tree_node);
  free (d->key);
  free (d);
}

static void
init (void)
{
  map = AVLTREE_INIT;
}

static inline void
clear (void)
{
  avltree_for_each (&map, dtor);
  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
}

static inline data *
data_insert (avltree_t *tree, data *node)
{
  avltree_node_t *parent = NULL;
  avltree_node_t **inpos = &tree->root;
  avltree_node_t *tree_node = &node->tree_node;

  for (avltree_node_t *curr = tree->root; curr;)
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

  avltree_link (tree, inpos, parent, &node->tree_node);

  return node;
}

static inline data *
data_find (avltree_t *tree, const data *target)
{
  const avltree_node_t *tree_node = &target->tree_node;

  for (avltree_node_t *curr = tree->root; curr;)
    {
      int comp_ret = comp (tree_node, curr);

      if (comp_ret == 0)
        return container_of (curr, data, tree_node);

      curr = comp_ret < 0 ? curr->left : curr->right;
    }

  return NULL;
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

      data *node = data_find (&map, &temp);
      assert (node->val == ages[i]);
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
      data *node = data_insert (&map, new);

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
      // long rmpos = i;
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;

      temp.key = names[rmpos];

      data *node = data_find (&map, &temp);
      avltree_erase (&map, &node->tree_node);
      free (node->key);
      free (node);

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
