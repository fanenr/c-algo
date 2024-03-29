#include "common.h"
#include "hashtable.h"
#include "hashtable_ext.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 300000UL

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
  size_t hash = 0;
  for (size_t len = strlen (key); len; len--)
    hash += key[len - 1];
  return hash;
}

static inline void
init (void)
{
  map = HASHTABLE_INIT;
}

static inline void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  hashtable_for_each (&map, (hashtable_visit_t *)free);
  free (map.buckets);
  map = HASHTABLE_INIT;

  memset (names, 0, sizeof (char *) * N);
  memset (ages, 0, sizeof (int) * N);
}

static inline data *
data_insert (hashtable_t *ht, char *key, int val)
{
  data *node = malloc (sizeof (data));
  node->hash_node.hash = hash (key);
  node->key = key;
  node->val = val;

#define HT_INIT_CAP 8
#define HT_EXPAN_RATIO 2
#define HT_LOAD_FACTOR 0.8

  if (ht->size >= ht->cap * HT_LOAD_FACTOR)
    {
      size_t newcap = ht->cap * HT_EXPAN_RATIO;
      if (newcap < HT_INIT_CAP)
        newcap = HT_INIT_CAP;
      hashtable_node_t **newbkts
          = calloc (newcap, sizeof (hashtable_node_t *));
      hashtable_rehash (newbkts, newcap, ht);
      free (ht->buckets);
      ht->cap = newcap;
      ht->buckets = newbkts;
    }

#undef HT_LOAD_FACTOR
#undef HT_EXPAN_RATIO
#undef HT_INIT_CAP

  hashtable_insert (ht, &node->hash_node);

  return node;
}

static inline data *
data_find (const hashtable_t *ht, const char *key)
{
  size_t code = hash (key);
  hashtable_node_t *head = hashtable_head (ht, code);

  for (hashtable_node_t *curr = head; curr; curr = curr->next)
    {
      if (code == curr->hash)
        {
          data *curr_data = container_of (curr, data, hash_node);
          if (strcmp (key, curr_data->key) == 0)
            return curr_data;
        }
    }

  return NULL;
}

static inline double
bench_find (void)
{
  TIME_ST ();
  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;

      data *node = data_find (&map, names[i]);
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
      data *node = data_insert (&map, names[i], ages[i]);

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

      data *node = data_find (&map, names[rmpos]);
      hashtable_erase (&map, &node->hash_node);
      free (node);

      free (names[rmpos]);
      names[rmpos] = NULL;
    }
  TIME_ED ();

  return TIME_VAL ();
}
