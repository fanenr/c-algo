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

vector *
vector_reserve (vector *vec, size_t cap, size_t ele)
{
  if (vec->cap >= cap)
    return vec;

  size_t ncap = vec->cap;
  if (ncap < VECTOR_INIT_CAP)
    ncap = VECTOR_INIT_CAP;

  while (ncap < cap)
    ncap *= VECTOR_EXPAN_RATIO;

  void *ndat = realloc (vec->data, ncap * ele);
  if (ndat == NULL)
    return NULL;

  vec->data = ndat;
  vec->cap = ncap;
  return vec;
}

void *
vector_at (vector *vec, size_t pos, size_t ele)
{
  if (pos >= vec->len)
    return NULL;
  return vec->data + pos * ele;
}

void *
vector_remove (vector *vec, size_t pos, size_t ele)
{
  if (pos >= vec->len)
    return NULL;

  if (pos == vec->len - 1)
    goto end;

  void *rmpos = vec->data + pos * ele;
  void *mvst = rmpos + ele;
  size_t mvlen = (vec->len - pos - 1) * ele;

  if (memmove (rmpos, mvst, mvlen) != rmpos)
    return NULL;

end:
  vec->len--;
  return rmpos;
}

void *
vector_push_back (vector *vec, void *data, size_t ele)
{
  if (!vector_reserve (vec, vec->len + 1, ele))
    return NULL;

  void *inpos = vec->data + vec->len * ele;

  if (memmove (inpos, data, ele) != inpos)
    return NULL;

  vec->len++;
  return inpos;
}

void *
vector_insert (vector *vec, size_t pos, void *data, size_t ele)
{
  if (pos > vec->len)
    return NULL;

  if (!vector_reserve (vec, vec->len + 1, ele))
    return NULL;

  if (pos == vec->len)
    return vector_push_back (vec, data, ele);

  void *inpos = vec->data + pos * ele;
  void *mvde = inpos + ele;
  size_t mvlen = (vec->len - pos) * ele;

  if (memmove (mvde, inpos, mvlen) != mvde)
    return NULL;

  if (memmove (inpos, data, ele) != inpos)
    return NULL;

  vec->len++;
  return inpos;
}
