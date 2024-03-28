#ifndef LIST_EXT_H
#define LIST_EXT_H

#include "list.h"

typedef void list_visit_t (list_node_t *n);
typedef int list_comp_t (const list_node_t *a, const list_node_t *b);

extern list_node_t *list_find (const list_t *list, const list_node_t *target,
                               list_comp_t *comp) attr_nonnull (1, 2, 3);

extern void list_for_each (list_t *list, list_visit_t *visit)
    attr_nonnull (1, 2);

#endif
