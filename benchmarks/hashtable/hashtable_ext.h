#ifndef HASHTABLE_EXT_H
#define HASHTABLE_EXT_H

#include "hashtable.h"

typedef int hashtable_comp_t (const hashtable_node_t *a,
                              const hashtable_node_t *b);
typedef void hashtable_visit_t (hashtable_node_t *n);

extern hashtable_node_t *hashtable_find (const hashtable_t *ht,
                                         const hashtable_node_t *target,
                                         hashtable_comp_t *comp)
    attr_nonnull (1, 2, 3);

extern void hashtable_for_each (hashtable_t *ht, hashtable_visit_t *visit)
    attr_nonnull (1, 2);

#endif
