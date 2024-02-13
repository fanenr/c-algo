#include "vector.h"
#include <stdlib.h>
#include <string.h>

void
vector_init (vector *vec)
{
  *vec = (vector){ .cap = 0, .len = 0, .data = NULL };
}

void
vector_free (vector *vec)
{
  free (vec->data);
  vector_init (vec);
  return;
}

void
vector_remove (vector *vec, size_t pos, const vector_i *info)
{
  if (pos >= vec->len)
    return;

  const size_t size = info->size;
  void *rmpos = vec->data + pos * size;
  if (pos == vec->len - 1)
    goto end;

  void *mvst = rmpos + size;
  size_t mvlen = (vec->len - pos - 1) * size;

  if (memmove (rmpos, mvst, mvlen) != rmpos)
    return;

end:
  vec->len--;
}

vector *
vector_reserve (vector *vec, size_t cap, const vector_i *info)
{
  if (vec->cap >= cap)
    return vec;

  size_t ncap = vec->cap;
  if (ncap < VECTOR_INIT_CAP)
    ncap = VECTOR_INIT_CAP;

  while (ncap < cap)
    ncap *= VECTOR_EXPAN_RATIO;

  void *ndat = realloc (vec->data, ncap * info->size);
  if (ndat == NULL)
    return NULL;

  vec->data = ndat;
  vec->cap = ncap;
  return vec;
}

void *
vector_at (vector *vec, size_t pos, const vector_i *info)
{
  if (pos >= vec->len)
    return NULL;
  return vec->data + pos * info->size;
}

void *
vector_push_back (vector *vec, void *data, const vector_i *info)
{
  if (!vector_reserve (vec, vec->len + 1, info))
    return NULL;

  const size_t size = info->size;
  void *inpos = vec->data + vec->len * size;

  if (memcpy (inpos, data, size) != inpos)
    return NULL;

  vec->len++;
  return inpos;
}

void *
vector_insert (vector *vec, size_t pos, void *data, const vector_i *info)
{
  if (pos > vec->len)
    return NULL;

  if (!vector_reserve (vec, vec->len + 1, info))
    return NULL;

  if (pos == vec->len)
    return vector_push_back (vec, data, info);

  const size_t size = info->size;
  void *inpos = vec->data + pos * size;
  void *mvde = inpos + size;
  size_t mvlen = (vec->len - pos) * size;

  if (memmove (mvde, inpos, mvlen) != mvde)
    return NULL;

  if (memcpy (inpos, data, size) != inpos)
    return NULL;

  vec->len++;
  return inpos;
}
