#include "vector.h"
#include <stdlib.h>
#include <string.h>

void
vector_init (vector_t *vec, size_t size, vector_comp_t *comp,
             vector_dtor_t *dtor)
{
  *vec = (vector_t){ .elem_size = size, .elem_comp = comp, .elem_dtor = dtor };
}

vector_t *
vector_reserve (vector_t *vec, size_t cap)
{
  size_t newcap = vec->cap;

  if (newcap >= cap)
    return vec;

  if (newcap < VECTOR_INIT_CAP)
    newcap = VECTOR_INIT_CAP;

  while (newcap < cap)
    newcap *= VECTOR_EXPAN_RATIO;

  void *newdata;
  size_t elem_size = vec->elem_size;

  if (!(newdata = realloc (vec->data, newcap * elem_size)))
    return NULL;

  vec->data = newdata;
  vec->cap = newcap;
  return vec;
}

void *
vector_find (const vector_t *vec, const void *target)
{
  void *curr = vec->data;
  size_t elem_size = vec->elem_size;
  vector_comp_t *const elem_comp = vec->elem_comp;

  if (!elem_comp)
    return NULL;

  for (size_t size = vec->size; size; size--)
    {
      if (elem_comp (target, curr) == 0)
        return curr;
      curr += elem_size;
    }

  return NULL;
}

void *
vector_at (const vector_t *vec, size_t pos)
{
  if (pos >= vec->size)
    return NULL;
  return vec->data + pos * vec->elem_size;
}

void *
vector_push_back (vector_t *vec)
{
  size_t size = vec->size;
  size_t elem_size = vec->elem_size;

  if (!vector_reserve (vec, size + 1))
    return NULL;

  void *end = vec->data + size * elem_size;

  vec->size++;
  return end;
}

void *
vector_insert (vector_t *vec, size_t pos)
{
  size_t size = vec->size;

  if (pos > size)
    return NULL;

  if (pos == size)
    return vector_push_back (vec);

  if (!vector_reserve (vec, size + 1))
    return NULL;

  size_t elem_size = vec->elem_size;
  void *in = vec->data + pos * elem_size;
  size_t len = (size - pos) * elem_size;
  void *in_next = in + elem_size;

  if (memmove (in_next, in, len) != in_next)
    return NULL;

  vec->size++;
  return in;
}

void
vector_remove (vector_t *vec, size_t pos)
{
  void *data = vec->data;
  size_t size = vec->size;
  size_t elem_size = vec->elem_size;
  void *rm = data + pos * elem_size;
  vector_dtor_t *const elem_dtor = vec->elem_dtor;

  if (pos >= size)
    return;

  if (elem_dtor)
    elem_dtor (rm);

  if (pos == size - 1)
    goto dec_size;

  void *rm_next = rm + elem_size;
  size_t len = (size - pos - 1) * elem_size;

  if (memmove (rm, rm_next, len) != rm)
    return;

dec_size:
  vec->size--;
}

void
vector_sort (vector_t *vec)
{
  size_t size = vec->size;
  vector_comp_t *elem_comp = vec->elem_comp;

  if (size < 2 || !elem_comp)
    return;

  qsort (vec->data, size, vec->elem_size, elem_comp);
}

void
vector_free (vector_t *vec)
{
  void *data = vec->data;
  size_t elem_size = vec->elem_size;
  vector_dtor_t *const elem_dtor = vec->elem_dtor;

  if (elem_dtor)
    {
      void *rm = data - elem_size;
      for (size_t size = vec->size; size; size--)
        elem_dtor ((rm += elem_size));
    }

  free (data);
  vector_init (vec, elem_size, vec->elem_comp, elem_dtor);
}
