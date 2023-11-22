/* binary-search tree */

#ifndef BSTREE_H
#define BSTREE_H

#include <stddef.h>
#include <stdint.h>

/* bstree node */
struct bstree_n
{
    struct bstree_n *left;
    struct bstree_n *right;
    struct bstree_n *parent;
    union {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;

        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;

        float f32;
        double f64;

        void *ptr;
        char buf[8];
    } data;
};

/*
 * comp receives two parameters: a left node and a right node. and it
 * returns a negative number to mean that the left node is smaller than the
 * right node, a positive number to indicate that the left node is greater than
 * the right node, and zero to represent that the two nodes are equal.
 */
typedef int (*bstree_comp)(const struct bstree_n *left,
                           const struct bstree_n *right);

/* bstree container (header) */
struct bstree_s
{
    size_t size;
    bstree_comp comp;
    struct bstree_n *root;
};

typedef struct bstree_s bstree;

/*
 * init `tree`.
 * `tree` needs a comparer `comp` to do the node comparison work (needed by
 * bstree_find, bstree_insert, bstree_remove).
 */
extern bstree *bstree_init(bstree *tree, bstree_comp comp);

/*
 * release `tree`.
 */
extern void bstree_free(bstree *tree);

/*
 * allocate a node.
 */
extern struct bstree_n *bstree_node(void);

/*
 * find `node` in `tree`.
 * `node` will be returned if found, and NULL will be returned if not.
 */
extern struct bstree_n *bstree_find(bstree *restrict tree,
                                    struct bstree_n *restrict node);

/*
 * insert `node` into `tree`.
 * `node` will be returned if insert successfully. otherwise, the insertion is
 * failed. if there is a duplicate node in `tree`, then that node will be
 * returned, or NULL will be returned if other errors occur.
 */
extern struct bstree_n *bstree_insert(bstree *restrict tree,
                                      struct bstree_n *restrict node);

/*
 * remove `node` in `tree`.
 * no nodes will be removed if `node` is NULL.
 */
extern void bstree_remove(bstree *restrict tree,
                          struct bstree_n *restrict node);

#endif