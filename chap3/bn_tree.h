/* binary-node tree */

#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdint.h>

/* tree node */
struct tree_n
{
    size_t height;
    struct tree_n *left;
    struct tree_n *right;
    struct tree_n *parent;
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

/* tree container (header) */
struct tree_s
{
    size_t size;
    struct tree_n *root;
};

typedef struct tree_s tree;

/*
 * init `tre`.
 */
extern tree *deque_init(tree *tre);

/*
 * release `tre`.
 */
extern void deque_free(tree *tre);

/*
 * allocate a node.
 */
extern struct tree_n *tree_node(void);

/*
 * insert `node` as the left child node of `pos` in `tre`.
 * no nodes will be inserted if `pos` is NULL. otherwise, after insertion,
 * `node` will take the place of the pos in the `tre`, and `pos` will become the
 * left child node of `node`.
 */
extern struct tree_n *tree_insert_left(tree *restrict tre,
                                       struct tree_n *restrict pos,
                                       struct tree_n *restrict node);
/*
 * insert `node` as the right child node of `pos` in `tre`.
 * no nodes will be inserted if `pos` is NULL. otherwise, after insertion,
 * `node` will take the place of the pos in the `tre`, and `pos` will become the
 * right child node of `node`.
 */
extern struct tree_n *tree_insert_right(tree *restrict tre,
                                        struct tree_n *restrict pos,
                                        struct tree_n *restrict node);

/*
 * remove the subtree with `pos` as the root node.
 * no nodes will be removed if `pos` is NULL.
 */
extern void tree_remove(tree *restrict tre, struct tree_n *restrict pos);

#endif