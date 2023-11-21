#include "bntree.h"
#include <stdlib.h>

bntree *bntree_init(bntree *tre)
{
    tre->size = 0;
    tre->root = NULL;
    return tre;
}

void bntree_free(bntree *tre)
{
    bntree_remove(tre, tre->root);
    bntree_init(tre);
}

struct bntree_n *bntree_node(void)
{
    struct bntree_n *node = malloc(sizeof(struct bntree_n));
    node->data.ptr = node->parent = node->left = node->right = NULL;
    return node;
}

struct bntree_n *bntree_insert_left(bntree *restrict tre,
                                    struct bntree_n *restrict pos,
                                    struct bntree_n *restrict node)
{
    if (pos == NULL)
        return NULL;

    node->left = pos->left;
    pos->left = node;

    tre->size++;
    return node;
}

struct bntree_n *bntree_insert_right(bntree *restrict tre,
                                     struct bntree_n *restrict pos,
                                     struct bntree_n *restrict node)
{
    if (pos == NULL)
        return NULL;

    node->right = pos->right;
    pos->right = node;

    tre->size++;
    return node;
}

void bntree_remove(bntree *restrict tre, struct bntree_n *restrict pos)
{
    if (pos == NULL)
        return;

    bntree_remove(tre, pos->left);
    bntree_remove(tre, pos->right);

    struct bntree_n *parent = pos->parent;
    free(pos);

    if (parent->left == pos)
        parent->left = NULL;
    if (parent->right == pos)
        parent->right = NULL;

    tre->size--;
    return;
}