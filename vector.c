#include "vector.h"
#include <stdlib.h>
#include <string.h>

vector_t *
vector_reserve (vector_t *vec, size_t cap)
{
  void *newdata;
  size_t newcap = vec->cap;
  size_t elem_size = vec->elem_size;

  if (newcap >= cap)
    return vec;

  if (newcap < VECTOR_INIT_CAP)
    newcap = VECTOR_INIT_CAP;

  while (newcap < cap)
    newcap *= VECTOR_EXPAN_RATIO;

  if (!(newdata = realloc (vec->data, newcap * elem_size)))
    return NULL;

  vec->data = newdata;
  vec->cap = newcap;
  return vec;
}

void *
vector_push_back (vector_t *vec)
{
  size_t size = vec->size;
  size_t elem_size = vec->elem_size;

  if (!vector_reserve (vec, size + 1))
    return NULL;

  vec->size++;
  return vec->data + size * elem_size;
}

void *
vector_insert (vector_t *vec, size_t pos)
{
  size_t size = vec->size;
  size_t elem_size = vec->elem_size;

  if (pos == size)
    return vector_push_back (vec);

  if (pos > size || !vector_reserve (vec, size + 1))
    return NULL;

  void *in = vec->data + pos * elem_size;
  size_t len = (size - pos) * elem_size;
  void *next = in + elem_size;

  if (memmove (next, in, len) != next)
    return NULL;

  vec->size++;
  return in;
}

void
vector_erase (vector_t *vec, size_t pos)
{
  size_t size = vec->size;
  void *rm = vector_at (vec, pos);
  size_t elem_size = vec->elem_size;

  if (!rm)
    return;

  if (pos == size - 1)
    goto dec_size;

  void *next = rm + elem_size;
  size_t len = (size - pos - 1) * elem_size;

  if (memmove (rm, next, len) != rm)
    return;

dec_size:
  vec->size--;
}

void *
vector_find (const vector_t *vec, const void *target, vector_comp_t *comp)
{
  void *curr = vec->data;
  size_t elem_size = vec->elem_size;

  for (size_t size = vec->size; size; size--)
    {
      if (comp (target, curr) == 0)
        return curr;
      curr += elem_size;
    }

  return NULL;
}

void
vector_sort (vector_t *vec, vector_comp_t *comp)
{
  size_t size = vec->size;

  if (size > 1)
    qsort (vec->data, size, vec->elem_size, comp);
}

void
vector_free (vector_t *vec, vector_dtor_t *dtor)
{
  void *data = vec->data;
  size_t elem_size = vec->elem_size;

  void *rm = data - elem_size;
  for (size_t size = vec->size; size; size--)
    dtor ((rm += elem_size));

  free (data);
  *vec = VECTOR_INIT (vec->elem_size);
}
