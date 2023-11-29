#include "hashst.h"

hashst *hashst_init(hashst *set, int type)
{
    set->head = NULL;
    set->vtype = type;
    set->size = set->capacity = 0;
    return set;
}
