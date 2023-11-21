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
    if (pos == NULL) {
        tre->root->parent = node;
        node->left = tre->root;
        tre->root = node;
    } else {
        pos->left->parent = node;
        node->left = pos->left;
        pos->left = node;
        node->parent = pos;
    }

    tre->size++;
    return node;
}

struct bntree_n *bntree_insert_right(bntree *restrict tre,
                                     struct bntree_n *restrict pos,
                                     struct bntree_n *restrict node)
{
    if (pos == NULL) {
        tre->root->parent = node;
        node->right = tre->root;
        tre->root = node;
    } else {
        pos->right->parent = node;
        node->right = pos->left;
        pos->right = node;
        node->parent = pos;
    }

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

    if (parent != NULL) {
        if (parent->left == pos)
            parent->left = NULL;
        if (parent->right == pos)
            parent->right = NULL;
    }

    tre->size--;
    return;
}