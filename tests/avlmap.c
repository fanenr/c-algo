#include "../avlmap.h"
#include "../common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100000

char *names[N];
int ages[N];

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

int
main (void)
{
  avlmap map;
  avlmap_init (&map);

  for (size_t i = 0; i < N; i++)
    {
      char *name = rand_string (rand_long (8, 17));
      assert (name);
      int age = rand_long (1, 101);
      names[i] = name;
      ages[i] = age;
      si_avlmap_n *node = si_avlmap_insert (&map, name, age);
      if (!node)
        names[i] = NULL;
    }

  balance_check (map.root);
  /* printf ("len: %lu height: %d\n", map.len, map.root->height); */

  for (size_t i = 0; i < N / 4; i++)
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

  balance_check (map.root);
  /* printf ("len: %lu height: %d\n", map.len, map.root->height); */

  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;
      si_avlmap_n *node = si_avlmap_find (&map, names[i]);
      assert (node->val == ages[i]);
    }

  for (size_t i = 0; i < N; i++)
    free (names[i]);
  avlmap_free (&map);
}