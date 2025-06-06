#include "common.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

#define T 1UL
#define N 300000UL

static void clear (void);

static double bench_find (void);
static double bench_insert (void);
static double bench_remove (void);

struct hash
{
  size_t
  operator() (char const *s) const noexcept
  {
    size_t hash = 0;
    for (size_t len = strlen (s); len; len--)
      hash += s[len - 1];
    return hash;
  }
};

struct comp
{
  bool
  operator() (char const *a, char const *b) const noexcept
  {
    return strcmp (a, b) == 0;
  }
};

int ages[N];
char *names[N];
std::unordered_map<char *, int, hash, comp> map;

int
main (void)
{
  rand_init ();

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

static inline void
clear (void)
{
  for (size_t i = 0; i < N; i++)
    free (names[i]);
  map.clear ();
}

static inline double
bench_find (void)
{
  TIME_ST ();
  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
	continue;

      auto const &found = map.find (names[i]);
      assert (found->second == ages[i]);
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
      char *key = names[i];
      auto pair = map.emplace (key, ages[i]);

      if (!pair.second)
	{
	  free (key);
	  names[i] = nullptr;
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
      auto const &iter = map.find (key);
      map.erase (iter);

      free (key);
      names[rmpos] = nullptr;
    }
  TIME_ED ();

  return TIME_VAL ();
}
