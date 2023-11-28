#include "../bntree.h"
#include <assert.h>
#include <stdlib.h>

static void test_init(void);
static void test_free(void);
static void test_node(void);
static void test_insert_left(void);
static void test_insert_right(void);
static void insert_helper(bntree *tree);
static void test_remove(void);

int main(void)
{
    test_init();
    test_free();
    test_node();
    test_insert_left();
    test_insert_right();
    test_remove();
    return 0;
}

static void test_init(void)
{
    bntree tree1;
    bntree_init(&tree1);

    assert(tree1.size == 0);
    assert(tree1.root == NULL);

    return;
}

static void test_free(void)
{
    /* release an empty bntree */
    bntree tree1;
    bntree_init(&tree1);
    bntree_free(&tree1);

    assert(tree1.size == 0);
    assert(tree1.root == NULL);

    /* release a bntree with some nodes */
    bntree tree2;
    bntree_init(&tree2);

    insert_helper(&tree2);

    bntree_free(&tree2);

    assert(tree2.size == 0);
    assert(tree2.root == NULL);

    /* remove(root) */
    bntree tree3;
    bntree_init(&tree3);

    insert_helper(&tree3);

    bntree_remove(&tree3, tree3.root);

    assert(tree3.size == 0);
    assert(tree3.root == NULL);

    return;
}

static void test_node(void)
{
    struct bntree_n *node;
    node = bntree_node();

    assert(node->left == NULL);
    assert(node->right == NULL);
    assert(node->parent == NULL);
    assert(node->data.ptr == NULL);

    free(node);
    return;
}

static void insert_helper(bntree *tree)
{
    struct bntree_n *parents[(1 << 10)];
    struct bntree_n *node;

    node = bntree_node();
    node->data.i32 = 0;
    parents[0] = bntree_insert_left(tree, NULL, node);

    for (int i = 1, temp = 1; i < 10; i++) {
        for (int j = 0; j < (1 << (i - 1)); j++) {
            node = bntree_node();
            node->data.i32 = temp;
            parents[temp] =
                bntree_insert_left(tree, parents[(temp - 1) / 2], node);
            temp++;

            node = bntree_node();
            node->data.i32 = temp;
            parents[temp] =
                bntree_insert_right(tree, parents[(temp - 1) / 2], node);
            temp++;
        }
    }

    return;
}

static void test_insert_left(void)
{
    bntree tree1;
    bntree_init(&tree1);

    insert_helper(&tree1);

    assert(tree1.size == (1 << 10) - 1);

    bntree_free(&tree1);

    assert(tree1.size == 0);
    assert(tree1.root == NULL);

    return;
}

static void test_insert_right(void)
{
    bntree tree1;
    bntree_init(&tree1);

    insert_helper(&tree1);

    assert(tree1.size == (1 << 10) - 1);

    bntree_free(&tree1);

    assert(tree1.size == 0);
    assert(tree1.root == NULL);

    return;
}

static void test_remove(void)
{
    bntree tree1;
    bntree_init(&tree1);

    insert_helper(&tree1);

    assert(tree1.size == (1 << 10) - 1);

    bntree_remove(&tree1, tree1.root->left);

    assert(tree1.size == 1 << 9);
    assert(tree1.root->left == NULL);

    bntree_free(&tree1);
    return;
}
