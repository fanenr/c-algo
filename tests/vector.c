#include "../vector.h"
#include <stdio.h>

struct bala
{
  int a;
  float b;
  long c;
  double d;
};

VECTOR_DEFS (int, i32);
VECTOR_DEFS (struct bala, bala);

int
main (void)
{
  vector vec;
  vector_init (&vec);

  for (int i = 0; i < 1000; i++)
    bala_vector_push_back (&vec, (struct bala){ .a = i });
  for (int i = 0; i < 1000; i++)
    printf ("%d ", bala_vector_at (&vec, i).a);

  vector_free (&vec);
}
