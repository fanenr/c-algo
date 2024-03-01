#include "../avlmap.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 1000000UL

avlmap map;
int ages[N];
char *names[N];

static void clear (void);
static void test_find (void);
static void test_insert (void);
static void test_remove (void);
static void check_bf (const avlmap_n *node);

static int si_comp (const char **a, const char **b);
AVLMAP_DEF_ALL (si, char *, int, si_comp);

int
main (void)
{
  for (size_t i = 0; i < T; i++)
    {
      test_insert ();
      check_bf (map.root);
      printf ("len: %lu\nheight: %d\n", map.size, map.root->height);

      test_remove ();
      check_bf (map.root);
      printf ("len: %lu\nheight: %d\n", map.size, map.root->height);

      test_find ();
      clear ();
    }
}

static inline int
si_comp (const char **a, const char **b)
{
  return strcmp (*a, *b);
}

static inline void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  avlmap_free (&map);

  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
}

static inline void
test_find (void)
{
  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;

      si_avlmap_n *node = si_avlmap_find (&map, names[i]);
      assert (node->val == ages[i]);
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
      si_avlmap_n *node = si_avlmap_insert (&map, names[i], ages[i]);
      if (!node)
        names[i] = NULL;
    }
}

static inline void
test_remove (void)
{
  for (size_t i = 0; i < N / 2; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;

      size_t size = map.size;
      si_avlmap_remove (&map, names[rmpos]);
      assert (map.size != size);

      free (names[rmpos]);
      names[rmpos] = NULL;
    }
}

static inline void
check_bf (const avlmap_n *node)
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
