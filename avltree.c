#include "avltree.h"

void
avltree_init (avltree *tree)
{
  *tree = (avltree){ .len = 0, .root = NULL };
}
