#include "bstree.h"
#include <stdlib.h>
#include <string.h>

bstree *bstree_init(bstree *tree, bstree_comp comp)
{
    tree->size = 0;
    tree->root = NULL;
    tree->comp = comp;
    return tree;
}

static void bstree_free_helper(struct bstree_n *node)
{
    if (node == NULL)
        return;
    bstree_free_helper(node->left);
    bstree_free_helper(node->right);
    free(node);
    return;
}

void bstree_free(bstree *tree)
{
    bstree_free_helper(tree->root);
    bstree_init(tree, tree->comp);
    return;
}

struct bstree_n *bstree_node(void)
{
    struct bstree_n *node = malloc(sizeof(struct bstree_n));
    node->data.ptr = node->parent = node->left = node->right = NULL;
    return node;
}
struct bstree_n *bstree_find(bstree *restrict tree,
                             struct bstree_n *restrict node)
{
    struct bstree_n *curr = tree->root;
    bstree_comp comp = tree->comp;
    int comp_val;

    if (node == NULL)
        return NULL;

    while (curr != NULL) {
        if (node == curr) /* node in tree */
            return curr;
        comp_val = comp(node, curr);
        if (comp_val == 0)
            return curr;
        else if (comp_val < 0)
            curr = curr->left;
        else if (comp_val > 0)
            curr = curr->right;
    }

    return NULL;
}

struct bstree_n *bstree_insert(bstree *restrict tree,
                               struct bstree_n *restrict node)
{
    struct bstree_n *curr = tree->root;
    bstree_comp comp = tree->comp;
    int comp_val;

    if (node == NULL)
        return NULL;

    if (curr == NULL) {
        tree->root = node;
        tree->size++;
        return node;
    }

    for (;;) {
        comp_val = comp(node, curr);
        if (comp_val == 0) /* duplicate */
            return curr;

        if (comp_val < 0) {
            if (curr->left == NULL) /* left */
                break;
            curr = curr->left;
        }

        if (comp_val > 0) {
            if (curr->right == NULL) /* right */
                break;
            curr = curr->right;
        }
    }

    node->parent = curr;
    if (comp_val < 0)
        curr->left = node;
    else
        curr->right = node;

    tree->size++;
    return node;
}

static struct bstree_n *bstree_find_min(struct bstree_n *root)
{
    if (root == NULL)
        return NULL;

    struct bstree_n *min = root;
    while (min->left != NULL)
        min = min->left;
    return min;
}

static struct bstree_n *bstree_find_max(struct bstree_n *root)
{
    if (root == NULL)
        return NULL;

    struct bstree_n *max = root;
    while (max->right != NULL)
        max = max->right;
    return max;
}

void bstree_remove(bstree *restrict tree, struct bstree_n *restrict node)
{
    int degree;
    struct bstree_n *find, *parent;
    struct bstree_n *temp;

    find = bstree_find(tree, node);
    if (find == NULL)
        return;

    parent = find->parent;
    degree = ((find->left != NULL) << 1) | (find->right != NULL);

    switch (degree) {
    case 0:                     /* leaf node */
        free(find);
        if (find == tree->root) /* remove root */
            tree->root = NULL;
        else if (parent->left == find)
            parent->left = NULL;
        else if (parent->right == find)
            parent->right = NULL;
        tree->size--;
        break;

    case 1: /* has right node */
        temp = find->right;
        free(find);
        if (find == tree->root) /* remove root */
            tree->root = temp;
        else if (parent->left == find)
            parent->left = temp;
        else if (parent->right == find)
            parent->right = temp;
        temp->parent = parent;
        tree->size--;
        break;

    case 2: /* has left node */
        temp = find->left;
        free(find);
        if (find == tree->root) /* remove root */
            tree->root = temp;
        else if (parent->left == find)
            parent->left = temp;
        else if (parent->right == find)
            parent->right = temp;
        temp->parent = parent;
        tree->size--;
        break;

    case 3: /* has tow nodes */
        temp = bstree_find_min(find->right);
        find->data = temp->data;
        if (temp->parent->left == temp)
            temp->parent->left = temp->right;
        else if (temp->parent->right == temp)
            temp->parent->right = temp->right;
        if (temp->right != NULL)
            temp->right->parent = temp->parent;
        free(temp);
        tree->size--;
        break;
    }

    return;
}