#include "../hashtable.h"
#include "../common.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1UL
#define N 100000UL

#define RM_ALL 0
#define USE_EXT 1

static void init (void);
static void clear (void);

static void test_find (void);
static void test_insert (void);
static void test_remove (void);

typedef struct data data;

struct data
{
  hashtable_node_t hash_node;
  const char *key;
  int val;
};

int ages[N];
char *names[N];
hashtable_t map;

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
  free ((char *) d->key);
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
  hashtable_visit (&map, dtor);
  free (map.data);
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

  hashtable_node_t **newdata = calloc (newcap, sizeof (hashtable_node_t *));
  hashtable_rehash (newdata, newcap, ht);
  free (ht->data);

  ht->data = newdata;
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
  data temp = (data) { .hash_node.hash = code, .key = (char *) key };

  const hashtable_node_t *target = &temp.hash_node;
  hashtable_node_t *head = hashtable_head (ht, code);

  for (hashtable_node_t *curr = head; curr; curr = curr->next)
    if (code == curr->hash && comp (target, curr) == 0)
      return container_of (curr, data, hash_node);

  return NULL;
}

static inline void
test_find (void)
{
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
}

#define data_new(data_key, data_val, hash_code)                               \
  ({                                                                          \
    data *new = malloc (sizeof (data));                                       \
    new->hash_node.hash = (hash_code);                                        \
    new->key = (data_key);                                                    \
    new->val = (data_val);                                                    \
    new;                                                                      \
  })

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
}

static inline void
test_remove (void)
{
  for (size_t i = 0; i < N; i++)
    {
#if RM_ALL
      long rmpos = i;
#else
      long rmpos = rand_long (0, N);
#endif

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
}
