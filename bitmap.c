#include "bitmap.h"

#define likely(exp) __builtin_expect (!!(exp), 1)
#define unlikely(exp) __builtin_expect (!!(exp), 0)
#define memset(src, c, n) __builtin_memset ((src), (c), (n))

void
bitmap_zero (bitmap_t *map)
{
  size_t size = BITMAP_BYTES (map->nbits);
  memset (map->data, 0, size);
  map->count = 0;
}

void
bitmap_fill (bitmap_t *map)
{
  size_t size = BITMAP_BYTES (map->nbits);
  memset (map->data, 0xff, size);
  map->count = map->nbits;
}

void
bitmap_set (bitmap_t *map, size_t pos)
{
  if (unlikely (pos >= map->nbits))
    return;

  size_t *s = map->data + pos / BITS_PER_ELEM;
  size_t mask = (1UL << (pos % BITS_PER_ELEM));

  if (!(*s & mask))
    map->count++;

  *s |= mask;
}

void
bitmap_flip (bitmap_t *map, size_t pos)
{
  if (unlikely (pos >= map->nbits))
    return;

  size_t *s = map->data + pos / BITS_PER_ELEM;
  size_t mask = (1UL << (pos % BITS_PER_ELEM));
  *s ^= mask;

  if (*s & mask)
    map->count++;
  else
    map->count--;
}

void
bitmap_clear (bitmap_t *map, size_t pos)
{
  if (unlikely (pos >= map->nbits))
    return;

  size_t *s = map->data + pos / BITS_PER_ELEM;
  size_t mask = (1UL << (pos % BITS_PER_ELEM));

  if (*s & mask)
    map->count--;

  *s &= ~mask;
}

int
bitmap_at (const bitmap_t *map, size_t pos)
{
  if (unlikely (pos >= map->nbits))
    return -1;

  size_t *s = map->data + pos / BITS_PER_ELEM;
  size_t mask = (1UL << (pos % BITS_PER_ELEM));
  size_t v = *s & mask;

  return !!v;
}

size_t
bitmap_find_first0 (const bitmap_t *map)
{
  size_t val, idx, elems = BITMAP_ELEMS (map->nbits);

  for (idx = 0; idx < elems; idx++)
    if ((val = map->data[idx]) != ~0UL)
      return idx * BITS_PER_ELEM + __builtin_ffsl (~val) - 1;

  return BITMAP_NONE;
}

size_t
bitmap_find_first1 (const bitmap_t *map)
{
  size_t val, idx, elems = BITMAP_ELEMS (map->nbits);

  for (idx = 0; idx < elems; idx++)
    if ((val = map->data[idx]) != 0UL)
      return idx * BITS_PER_ELEM + __builtin_ffsl (val) - 1;

  return BITMAP_NONE;
}
