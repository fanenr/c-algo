#include "../bstree.h"
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static void test_init(void);
static void test_free(void);
static void test_node(void);
static void test_insert(void);
static void test_remove(void);
static void traverse(const struct bstree_n *node);
static void insert_helper(bstree *tree, int num, int max);
static int comp(const struct bstree_n *node, const struct bstree_n *other);

int main(void)
{
    test_init();
    test_free();
    test_node();
    test_insert();
    test_remove();
    return 0;
}

static void traverse(const struct bstree_n *node)
{
    if (node == NULL)
        return;
    traverse(node->left);
    /* do something */
    traverse(node->right);
    return;
}

static void insert_helper(bstree *tree, int num, int max)
{
    srand(time(NULL));
    struct bstree_n *node;

    for (int i = 0; i < num; i++) {
        node = bstree_node();
        node->data.i32 = rand() % max;
        if (bstree_insert(tree, node) != node) { /* duplicate nodes */
            free(node);                          /* node is not inserted */
            i--;                                 /* generate again */
        }
    }
    
    return;
}

static int comp(const struct bstree_n *node, const struct bstree_n *other)
{
    if (node->data.i32 < other->data.i32)
        return -1;
    if (node->data.i32 > other->data.i32)
        return 1;
    return 0;
}

static void test_init(void)
{
    bstree tree1;
    bstree_init(&tree1, comp);

    assert(tree1.size == 0);
    assert(tree1.root == NULL);
    assert(tree1.comp == comp);

    return;
}

static void test_free(void)
{
    /* release an empty bntree */
    bstree tree1;
    bstree_init(&tree1, comp);
    bstree_free(&tree1);

    assert(tree1.size == 0);
    assert(tree1.root == NULL);

    /* release a bntree with some nodes */
    bstree tree2;
    bstree_init(&tree2, comp);

    insert_helper(&tree2, 1000, 10000);

    bstree_free(&tree2);

    assert(tree2.size == 0);
    assert(tree2.root == NULL);

    return;
}

static void test_node(void)
{
    struct bstree_n *node;
    node = bstree_node();

    assert(node->left == NULL);
    assert(node->right == NULL);
    assert(node->parent == NULL);
    assert(node->data.ptr == NULL);

    free(node);
    return;
}

static void test_insert(void)
{
    bstree tree1;
    bstree_init(&tree1, comp);

    insert_helper(&tree1, 10, 100);
    assert(tree1.size == 10);

    bstree_free(&tree1);
    return;
}

static void test_remove(void)
{
    bstree tree1;
    bstree_init(&tree1, comp);

    insert_helper(&tree1, 10, 100);
    assert(tree1.size == 10);

    for (int i = 0; i < 10; i++) {
        bstree_remove(&tree1, tree1.root);

        assert(tree1.size == 9 - i);
    }

    bstree_free(&tree1);
    return;
}