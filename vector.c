#include "vector.h"
#include <stdlib.h>
#include <string.h>

void
vector_init (vector_t *vec, size_t elem_size)
{
  *vec = (vector_t){ .elem_size = elem_size };
}

void
vector_free (vector_t *vec)
{
  free (vec->data);
  vec->data = NULL;
  vec->size = vec->cap = 0;
}

void
vector_remove (vector_t *vec, size_t pos)
{
  if (pos >= vec->size)
    return;

  if (pos == vec->size - 1)
    goto ok;

  size_t esize = vec->elem_size;
  void *rmpos = vec->data + pos * esize;
  size_t mvlen = (vec->size - pos - 1) * esize;

  if (memmove (rmpos, rmpos + esize, mvlen) != rmpos)
    return;

ok:
  vec->size--;
}

vector_t *
vector_reserve (vector_t *vec, size_t cap)
{
  if (vec->cap >= cap)
    return vec;

  void *newdata;
  size_t newcap = vec->cap;
  size_t esize = vec->elem_size;

  if (newcap < VECTOR_INIT_CAP)
    newcap = VECTOR_INIT_CAP;

  while (newcap < cap)
    newcap *= VECTOR_EXPAN_RATIO;

  if (!(newdata = realloc (vec->data, newcap * esize)))
    return NULL;

  vec->data = newdata;
  vec->cap = newcap;
  return vec;
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
  size_t esize = vec->elem_size;

  if (!vector_reserve (vec, vec->size + 1))
    return NULL;

  void *end = vec->data + vec->size * esize;

  vec->size++;
  return end;
}

void *
vector_insert (vector_t *vec, size_t pos)
{
  if (pos > vec->size)
    return NULL;

  if (pos == vec->size)
    return vector_push_back (vec);

  if (!vector_reserve (vec, vec->size + 1))
    return NULL;

  size_t esize = vec->elem_size;
  void *in = vec->data + pos * esize;
  size_t len = (vec->size - pos) * esize;

  if (memmove (in + esize, in, len) != in + esize)
    return NULL;

  vec->size++;
  return in;
}
