#include <cassert>
#include <map>
#include <cstdio>
#include <cstring>
#include "common.h"

#define N 10000000

char *names[N];
int ages[N];

struct comp
{
  bool
  operator() (char const *a, char const *b) const
  {
    return strcmp (a, b) < 0;
  }
};

std::map<char *, int, comp> map;

static void
bench_insert (void)
{
  for (size_t i = 0; i < N; i++)
    {
      char *name = rand_string (rand_long (8, 17));
      assert (name);
      int age = rand_long (1, 101);
      names[i] = name;
      ages[i] = age;
    }

  TIME_ST ();
  for (size_t i = 0; i < N; i++)
    {
      auto pair = map.emplace (names[i], ages[i]);
      if (!pair.second)
        names[i] = nullptr;
    }
  TIME_ED ();

  printf ("insert: %lf\n", TIME_VAL ());
}

static void
bench_remove (void)
{
  TIME_ST ();
  for (size_t i = 0; i < N / 2; i++)
    {
      long rmpos = rand_long (0, N);
      if (!names[rmpos])
        continue;
      map.erase (names[rmpos]);
      names[rmpos] = nullptr;
    }
  TIME_ED ();

  printf ("remove: %lf\n", TIME_VAL ());
}

static void
bench_find (void)
{
  TIME_ST ();
  for (size_t i = 0; i < N; i++)
    {
      if (!names[i])
        continue;
      auto const &found = map.find (names[i]);
      if (found->second != ages[i])
        {
          printf ("find failed\n");
          abort ();
        }
    }
  TIME_ED ();

  printf ("find: %lf\n", TIME_VAL ());
}

int
main (void)
{
  bench_insert ();
  bench_remove ();
  bench_find ();
}
