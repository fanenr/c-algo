#include "../vector.h"
#include <assert.h>

struct bala
{
  int a;
  float b;
  long c;
  double d;
};

VECTOR_DEF_ALL (i32, int);
VECTOR_DEF_ALL (bala, struct bala);

int
main (void)
{
  bala_vector vec;
  bala_vector_init (&vec);

  for (int i = 0; i < 1000; i++)
    bala_vector_push_back (&vec, (struct bala){ .a = i });

  assert (vec.size == 1000);
  for (int i = 0; i < 1000; i++)
    assert (bala_vector_at (&vec, i)->a == i);

  bala_vector_free (&vec);
}
