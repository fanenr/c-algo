#include "common.h"
#include "hashtable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 300000UL

#define USE_EXT 1

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
  free ((char *)d->key);
  free (d);
}

static inline void
init (void)
{
  map = HASHTABLE_INIT;
}

static inline void
clear (void)
{
  hashtable_visit (&map, dtor);
  free (map.data);
  map = HASHTABLE_INIT;
}

static inline void
hashtable_expand (hashtable_t *ht)
{
#define HT_INIT_CAP 8
#define HT_EXPAN_RATIO 2
#define HT_LOAD_FACTOR 0.8

  if (ht->size < ht->cap * HT_LOAD_FACTOR)
    return;

  size_t newcap = ht->cap * HT_EXPAN_RATIO;
  if (newcap < HT_INIT_CAP)
    newcap = HT_INIT_CAP;

  hashtable_node_t **newslots = calloc (newcap, sizeof (hashtable_node_t *));
  hashtable_rehash (newslots, newcap, ht);
  free (ht->data);

  ht->data = newslots;
  ht->cap = newcap;

#undef HT_LOAD_FACTOR
#undef HT_EXPAN_RATIO
#undef HT_INIT_CAP
}

static inline data *
data_hashtable_insert (hashtable_t *ht, char *key, int val)
{
  size_t code = hash (key);

  data *new = malloc (sizeof (data));
  new->hash_node.hash = code;
  new->key = key;
  new->val = val;

  hashtable_node_t *prev = NULL;
  hashtable_node_t *hash_node = &new->hash_node;
  hashtable_node_t **head = ht->data + code % ht->cap;

  for (hashtable_node_t *curr = *head; curr; curr = curr->next)
    {
      if (code == curr->hash && comp (hash_node, curr) == 0)
        {
          free (new);
          return NULL;
        }
      prev = curr;
    }

  hashtable_node_t **inpos = prev ? &prev->next : head;
  hashtable_link (ht, inpos, prev, hash_node);
  return new;
}

static inline data *
data_hashtable_find (hashtable_t *ht, const char *key)
{
  size_t code = hash (key);
  data temp = (data){ .hash_node.hash = code, .key = (char *)key };

  const hashtable_node_t *target = &temp.hash_node;
  hashtable_node_t *head = hashtable_head (ht, code);

  for (hashtable_node_t *curr = head; curr; curr = curr->next)
    if (code == curr->hash && comp (target, curr) == 0)
      return container_of (curr, data, hash_node);

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

      char *key = names[i];

#if USE_EXT
      data temp = { .hash_node.hash = hash (key), .key = key };
      hashtable_node_t *node = hashtable_find (&map, &temp.hash_node, comp);
      data *d = container_of (node, data, hash_node);
#else
      data *d = data_hashtable_find (&map, key);
#endif

      assert (d->val == ages[i]);
    }
  TIME_ED ();

  return TIME_VAL ();
}

#define data_new(data_key, data_val, hash_code)                               \
  ({                                                                          \
    data *new = malloc (sizeof (data));                                       \
    new->hash_node.hash = (hash_code);                                        \
    new->key = (data_key);                                                    \
    new->val = (data_val);                                                    \
    new;                                                                      \
  })

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
      hashtable_expand (&map);

      char *key = names[i];

#if USE_EXT
      data *new = data_new (key, ages[i], hash (key));
      hashtable_node_t *node = hashtable_insert (&map, &new->hash_node, comp);
#else
      data *node = data_hashtable_insert (&map, key, ages[i]);
#endif

      if (!node)
        {
          free (names[i]);
          names[i] = NULL;
        }
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

      char *key = names[rmpos];

#ifdef USE_EXT
      data temp = { .hash_node.hash = hash (key), .key = key };

      hashtable_node_t *node = hashtable_find (&map, &temp.hash_node, comp);
      hashtable_erase (&map, node);
      dtor (node);
#else
      data *d = data_hashtable_find (&map, key);
      hashtable_erase (&map, &d->hash_node);
      dtor (&d->hash_node);
#endif

      names[rmpos] = NULL;
    }
  TIME_ED ();

  return TIME_VAL ();
}
