#include "vector.h"
#include <stdlib.h>
#include <string.h>

void
vector_init (vector *vec)
{
  memset (vec, 0, sizeof (vector));
}

void
vector_free (vector *vec)
{
  free (vec->data);
  vector_init (vec);
}

void
vector_remove (vector *vec, size_t pos, const vector_i info)
{
  if (pos >= vec->size)
    return;

  if (pos == vec->size - 1)
    goto end;

  size_t size = info.d_size;
  void *rmpos = vec->data + pos * size;
  size_t mvlen = (vec->size - pos - 1) * size;

  if (memmove (rmpos, rmpos + size, mvlen) != rmpos)
    return;

end:
  vec->size--;
}

vector *
vector_reserve (vector *vec, size_t cap, const vector_i info)
{
  if (vec->cap >= cap)
    return vec;

  size_t ncap = vec->cap;
  if (ncap < VECTOR_INIT_CAP)
    ncap = VECTOR_INIT_CAP;

  while (ncap < cap)
    ncap *= VECTOR_EXPAN_RATIO;

  void *ndat = realloc (vec->data, ncap * info.d_size);
  if (ndat == NULL)
    return NULL;

  vec->data = ndat;
  vec->cap = ncap;
  return vec;
}

void *
vector_at (const vector *vec, size_t pos, const vector_i info)
{
  if (pos >= vec->size)
    return NULL;
  return vec->data + pos * info.d_size;
}

void *
vector_push_back (vector *vec, void *data, const vector_i info)
{
  if (!vector_reserve (vec, vec->size + 1, info))
    return NULL;

  size_t size = info.d_size;
  void *inpos = vec->data + vec->size * size;

  if (memcpy (inpos, data, size) != inpos)
    return NULL;

  vec->size++;
  return inpos;
}

void *
vector_insert (vector *vec, size_t pos, void *data, const vector_i info)
{
  if (pos > vec->size)
    return NULL;

  if (pos == vec->size)
    return vector_push_back (vec, data, info);

  if (!vector_reserve (vec, vec->size + 1, info))
    return NULL;

  size_t size = info.d_size;
  void *inpos = vec->data + pos * size;
  size_t mvlen = (vec->size - pos) * size;

  if (memmove (inpos + size, inpos, mvlen) != inpos + size)
    return NULL;

  if (memcpy (inpos, data, size) != inpos)
    return NULL;

  vec->size++;
  return inpos;
}
