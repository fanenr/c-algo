#include "../avlmap.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100000

char *names[N];
int ages[N];

avlmap map;

int
si_avlmap_comp (const char **key1, const char **key2)
{
  const char *s1 = *key1;
  const char *s2 = *key2;
  return strcmp (s1, s2);
}

AVLMAP_DEF_ALL (char *, int, si);

static void
balance_check (const avlmap_n *node)
{
  if (!node)
    return;

  balance_check (node->left);
  balance_check (node->right);

  int lh = node->left ? node->left->height : -1;
  int rh = node->right ? node->right->height : -1;
  int bf = lh - rh;
  assert (bf >= -1 && bf <= 1);
}

static void
init (void)
{
  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
  avlmap_init (&map);
}

static void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  avlmap_free (&map);
}

static void
test_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      char *name = NULL;
      assert ((name = rand_string (rand_long (8, 17))));
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

static void
test_remove (void)
{
  for (size_t i = 0; i < N / 2; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;

      size_t len = map.len;
      si_avlmap_remove (&map, names[rmpos]);
      assert (map.len != len);

      free (names[rmpos]);
      names[rmpos] = NULL;
    }
}

static void
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

int
main (void)
{
  size_t times = 1;
  for (size_t i = 0; i < times; i++)
    {
      init ();

      test_insert ();
      balance_check (map.root);
      printf ("len: %lu\nheight: %d\n", map.len, map.root->height);

      test_remove ();
      balance_check (map.root);
      printf ("len: %lu\nheight: %d\n", map.len, map.root->height);

      test_find ();

      clear ();
    }
}
