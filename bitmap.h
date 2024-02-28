#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>

#define BITS_PER_BYTE 8
#define BITS_PER_ELEM (sizeof (size_t) * BITS_PER_BYTE)

#define BITMAP_ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))
#define BITMAP_ELEMS(n) (BITMAP_ALIGN (n, BITS_PER_ELEM) / BITS_PER_ELEM)
#define BITMAP_BYTES(n) (BITMAP_ALIGN (n, BITS_PER_ELEM) / BITS_PER_BYTE)

#define attr_nonnull(...) __attribute__ ((nonnull (__VA_ARGS__)))

typedef struct bitmap_t bitmap_t;

struct bitmap_t
{
  size_t *data;
  size_t count;
  size_t nbits;
};

#define BITMAP_NONE ((size_t) - 1)

#define BITMAP_INIT                                                           \
  (bitmap_t) {}

extern void bitmap_zero (bitmap_t *map) attr_nonnull (1);
extern void bitmap_fill (bitmap_t *map) attr_nonnull (1);

extern void bitmap_set (bitmap_t *map, size_t pos) attr_nonnull (1);
extern void bitmap_flip (bitmap_t *map, size_t pos) attr_nonnull (1);
extern void bitmap_clear (bitmap_t *map, size_t pos) attr_nonnull (1);

/* if @pos >= @map->nbits, -1 is returned. */
extern int bitmap_at (const bitmap_t *map, size_t pos) attr_nonnull (1);

/* if no 0-bits in @map, BITMAP_NONE is returned. */
extern size_t bitmap_find_first0 (const bitmap_t *map) attr_nonnull (1);

/* if no 1-bits in @map, BITMAP_NONE is returned. */
extern size_t bitmap_find_first1 (const bitmap_t *map) attr_nonnull (1);

#endif
