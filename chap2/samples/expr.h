#ifndef EXPR_H
#define EXPR_H

#include "../slist.h"

enum EXPR_NODE_TYPE
{
    EXPR_NODE_PLUS = '+',  /* operator: + */
    EXPR_NODE_MINUS = '-', /* operator: - */
    EXPR_NODE_STAR = '*',  /* operator: * */
    EXPR_NODE_SLASH = '/', /* operator: / */

    EXPR_NODE_INT = -1,    /* operand: integer */
    EXPR_NODE_FLOAT = -2,  /* operand: floating */
};

struct expr_node
{
    enum EXPR_NODE_TYPE type;
    union {
        long integer;
        double floating;
    } data;
};

/*
 * converts an infix expression to a postfix expression.
 * read no more than `len` chars from `src` and try to convert it.
 * return NULL on failure and slist on success
 */
extern slist in2post(char *src, size_t len);

/*
 * free the `list`.
 * ensure all nodes of `list` contain a pointer of expr_node and the expr_node
 * must be allocated by malloc.
 */
extern void expr_slist_free(slist *list);

#endif