#include "bntree.h"
#include <stdlib.h>

bntree *bntree_init(bntree *tree)
{
    tree->size = 0;
    tree->root = NULL;
    return tree;
}

void bntree_free(bntree *tree)
{
    bntree_remove(tree, tree->root);
    bntree_init(tree);
    return;
}

struct bntree_n *bntree_node(void)
{
    struct bntree_n *node = malloc(sizeof(struct bntree_n));
    node->data.ptr = node->parent = node->left = node->right = NULL;
    return node;
}

struct bntree_n *bntree_insert_left(bntree *restrict tree,
                                    struct bntree_n *restrict pos,
                                    struct bntree_n *restrict node)
{
    if (pos == NULL) {
        node->left = tree->root;
        if (tree->root != NULL)
            tree->root->parent = node;
        tree->root = node;
    } else {
        node->parent = pos;
        node->left = pos->left;
        if (pos->left != NULL)
            pos->left->parent = node;
        pos->left = node;
    }

    tree->size++;
    return node;
}

struct bntree_n *bntree_insert_right(bntree *restrict tree,
                                     struct bntree_n *restrict pos,
                                     struct bntree_n *restrict node)
{
    if (pos == NULL) {
        node->right = tree->root;
        if (tree->root != NULL)
            tree->root->parent = node;
        tree->root = node;
    } else {
        node->parent = pos;
        node->right = pos->right;
        if (pos->right != NULL)
            pos->right->parent = node;
        pos->right = node;
    }

    tree->size++;
    return node;
}

void bntree_remove(bntree *restrict tree, struct bntree_n *restrict pos)
{
    if (pos == NULL)
        return;

    bntree_remove(tree, pos->left);
    bntree_remove(tree, pos->right);

    struct bntree_n *parent = pos->parent;
    free(pos);

    if (parent != NULL) {
        if (parent->left == pos)
            parent->left = NULL;
        if (parent->right == pos)
            parent->right = NULL;
    } else {
        tree->root = NULL;
    }

    tree->size--;
    return;
}
