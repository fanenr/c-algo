#include "../vector.h"
#include <assert.h>

struct bala
{
  int a;
  float b;
  long c;
  double d;
};

VECTOR_DEF_ALL (int, i32);
VECTOR_DEF_ALL (struct bala, bala);

int
main (void)
{
  vector vec;
  vector_init (&vec);

  for (int i = 0; i < 1000; i++)
    bala_vector_push_back (&vec, (struct bala){ .a = i });

  assert (vec.len == 1000);
  for (int i = 0; i < 1000; i++)
    assert (bala_vector_at (&vec, i)->a == i);

  vector_free (&vec);
}
