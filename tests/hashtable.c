#include "../hashtable.h"
#include "../common.h"
#include "../hashtable_ext.h"

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

typedef struct data data;

struct data
{
  hashtable_node_t hash_node;
  char *key;
  int val;
};

int ages[N];
char *names[N];
hashtable_t map;

#define data_hashtable_node_new(data_key, data_val)                           \
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
      printf ("size: %lu\ncap: %lu\n", map.size, map.cap);

      test_find ();

      test_remove ();
      printf ("size: %lu\ncap: %lu\n", map.size, map.cap);

      test_find ();

      clear ();
    }
}

static inline size_t
hash (const char *key)
{
  /*
    size_t hash = 0;
    for (size_t len = strlen (key); len; len--)
      hash += key[len - 1];
    return hash;
  */
  return (size_t)key;
}

static inline int
comp (const hashtable_node_t *a, const hashtable_node_t *b)
{
  const data *da = container_of (a, data, hash_node);
  const data *db = container_of (b, data, hash_node);
  return strcmp (da->key, db->key);
}

static inline void
dtor (hashtable_node_t *n)
{
  data *d = container_of (n, data, hash_node);
  free (d->key);
  free (d);
}

static void
init (void)
{
  map = HASHTABLE_INIT;
}

static inline void
clear (void)
{
  hashtable_for_each (&map, dtor);
  free (map.data);
  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
}

static inline data *
data_insert (hashtable_t *ht, data *node)
{
#define HT_INIT_CAP 8
#define HT_EXPAN_RATIO 2
#define HT_LOAD_FACTOR 0.8

  if (ht->size >= ht->cap * HT_LOAD_FACTOR)
    {
      size_t newcap = ht->cap * HT_EXPAN_RATIO;
      if (newcap < HT_INIT_CAP)
        newcap = HT_INIT_CAP;
      hashtable_node_t **newdata
          = calloc (newcap, sizeof (hashtable_node_t *));
      hashtable_move (newdata, newcap, ht);
      free (ht->data);
      ht->cap = newcap;
      ht->data = newdata;
    }

#undef HT_LOAD_FACTOR
#undef HT_EXPAN_RATIO
#undef HT_INIT_CAP

  hashtable_insert (ht, hash (node->key), &node->hash_node);

  return node;
}

static inline data *
data_find (hashtable_t *ht, const data *target)
{
  const hashtable_node_t *hash_node = &target->hash_node;
  size_t hash = hash_node->hash;

  hashtable_node_t *head = hashtable_head (ht, hash);

  for (hashtable_node_t *curr = head; curr; curr = curr->next)
    if (hash == curr->hash && comp (hash_node, curr) == 0)
      return container_of (curr, data, hash_node);

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
      temp.hash_node.hash = hash (names[i]);

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
      data *new = data_hashtable_node_new (names[i], ages[i]);
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
      temp.hash_node.hash = hash (names[rmpos]);

      data *node = data_find (&map, &temp);
      hashtable_erase (&map, &node->hash_node);
      free (node->key);
      free (node);

      names[rmpos] = NULL;
    }
}
