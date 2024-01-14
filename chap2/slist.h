/* single-linked list */

#ifndef SLIST_H
#define SLIST_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct slist_v
{
  union
  {
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;

    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;

    float f32;
    double f64;

    void *ptr;
    char buf[8];
  };
};

/* slist node */
struct slist_n
{
  struct slist_n *next;
  struct slist_v data;
};

/* slist container (header) */
typedef struct
{
  size_t size;
  struct slist_n *head;
} slist;

/*
 * init `list`.
 */
extern slist *slist_init (slist *list);

/*
 * release `list`.
 */
extern void slist_free (slist *list);

/*
 * allocate a node.
 */
extern struct slist_n *slist_node (void);

/*
 * find the first node whose data equals to `data` from `spos` (include
 * `spos`). a pointer of that found node will be returned if found, otherwise
 * NULL will be returend.
 */
extern struct slist_n *slist_find_from (slist *restrict list,
                                        struct slist_n *restrict spos,
                                        struct slist_v data);

/*
 * find the first node whose data equals to `data` in `list`.
 */
extern struct slist_n *slist_find (slist *list, struct slist_v data);

/*
 * insert a node after `pos` into `list`.
 * a pointer of inserted node will be returned if insert successfully,
 * otherwise NULL will be returned.
 */
extern struct slist_n *slist_insert_after (slist *restrict list,
                                           struct slist_n *restrict pos,
                                           struct slist_v data);

/*
 * insert a node into the front of `list`.
 * a pointer of inserted node will be returned if insert successfully,
 * otherwise NULL will be returned.
 */
extern struct slist_n *slist_push_front (slist *restrict list,
                                         struct slist_v data);

/*
 * remove the node at `pos` in `list`.
 * `pos` will be returned if erased successfully, otherwise NULL will be
 * returned.
 */
extern struct slist_n *slist_erase (slist *restrict list,
                                    struct slist_n *restrict pos);

/*
 * remove the first node whose data equals to `data` from `spos`
 * (include `spos`).
 */
extern void slist_remove_from (slist *restrict list,
                               struct slist_n *restrict spos,
                               struct slist_v data);

/*
 * remove the first node whose data equals to `data` in `list`.
 */
extern void slist_remove (slist *list, struct slist_v data);

/*
 * remove the front node of `list`.
 */
extern void slist_pop_front (slist *list);

static inline slist
slist_new (void)
{
  slist list;
  slist_init (&list);
  return list;
}

static inline size_t
slist_size (const slist *list)
{
  return list == NULL ? 0 : list->size;
}

static inline struct slist_n *
slist_head (const slist *list)
{
  return list == NULL ? NULL : list->head;
}

#endif
