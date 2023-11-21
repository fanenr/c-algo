/* binary-node tree */

#ifndef BNTREE_H
#define BNTREE_H

#include <stddef.h>
#include <stdint.h>

/* bntree node */
struct bntree_n
{
    struct bntree_n *left;
    struct bntree_n *right;
    struct bntree_n *parent;
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

/* bntree container (header) */
struct bntree_s
{
    size_t size;
    struct bntree_n *root;
};

typedef struct bntree_s bntree;

/*
 * init `tre`.
 */
extern bntree *bntree_init(bntree *tre);

/*
 * release `tre`.
 */
extern void bntree_free(bntree *tre);

/*
 * allocate a node.
 */
extern struct bntree_n *bntree_node(void);

/*
 * insert `node` as the left child node of `pos` in `tre`.
 * if `pos` is NULL, `node` will become the root node of `tre`, and the previous
 * root will become the left child of `node`.
 * if not, `node` will take the place of `pos` in the `tre`, and `pos` will
 * become the left child of `node`.
 */
extern struct bntree_n *bntree_insert_left(bntree *restrict tre,
                                           struct bntree_n *restrict pos,
                                           struct bntree_n *restrict node);

/*
 * insert `node` as the right child node of `pos` in `tre`.
 * if `pos` is NULL, `node` will become the root node of `tre`, and the previous
 * root will become the right child of `node`.
 * if not, `node` will take the place of `pos` in the `tre`, and `pos` will
 * become the left right of `node`.
 */
extern struct bntree_n *bntree_insert_right(bntree *restrict tre,
                                            struct bntree_n *restrict pos,
                                            struct bntree_n *restrict node);

/*
 * remove the subtree with `pos` as the root node.
 * no nodes will be removed if `pos` is NULL.
 */
extern void bntree_remove(bntree *restrict tre, struct bntree_n *restrict pos);

#endif