#include "common.h"
#include "hashtable.h"

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

static inline size_t
hash (const char *key)
{
  return (size_t)key;
}

static inline int
comp (const hashtable_node_t *a, const hashtable_node_t *b)
{
  const data *da = container_of (a, data, hash_node);
  const data *db = container_of (b, data, hash_node);
  return strcmp (da->key, db->key);
}

static void
init (void)
{
  map = HASHTABLE_INIT;
}

static inline void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  free (map.data);
  map = HASHTABLE_INIT;

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
      temp.hash_node.hash = hash (names[i]);

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
      data *new = data_hashtable_node_new (names[i], ages[i]);
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
      temp.hash_node.hash = hash (names[rmpos]);

      data *node = data_find (&map, &temp);
      hashtable_erase (&map, &node->hash_node);
      free (node);

      names[rmpos] = NULL;
    }
  TIME_ED ();

  return TIME_VAL ();
}
