#include "expr.h"
#include "../stack.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static struct expr_node *alloc_expr_node()
{
    struct expr_node *node;
    node = malloc(sizeof(struct expr_node));
    assert(node != NULL);

    node->type = EXPR_NODE_INT;
    node->data.integer = 0;
    return node;
}

slist in2post(const char *src, size_t len)
{
    stack stac;
    slist list;
    stack_init(&stac);
    slist_init(&list);

    struct stack_n stac_node, *stac_top;
    struct slist_n *list_node, *list_tail;
    list_tail = NULL;

    long tmpi;
    double tmpf;
    char *endi, *endf;
    struct expr_node *enode;

#define LAST_READ_START 0
#define LAST_READ_SYMBOL 1
#define LAST_READ_NUMBER 2
#define LAST_READ_BRACKET 3
#define LAST_READ_OPERATOR 4

    int symbol_bit = 1;
    int last_read = LAST_READ_START;

#define save_expr_node(plist, enode, lnode, ltail)                             \
    enode = alloc_expr_node();                                                 \
    lnode = slist_node();                                                      \
    lnode->data.ptr = enode;                                                   \
    ltail = slist_insert(plist, ltail, lnode);

    for (int i = 0; i < len; i++) {
        switch (src[i]) {
        case '0' ... '9':
            tmpf = strtod(src + i, &endf);
            tmpi = strtol(src + i, &endi, 10);
            save_expr_node(&list, enode, list_node, list_tail);

            if (endf != src + i && *endi == '.') { /* got a floating */
                enode->data.floating =
                    last_read == LAST_READ_SYMBOL ? tmpf * symbol_bit : tmpf;
                enode->type = EXPR_NODE_FLOAT;
                i = endf - src - 1;
            } else if (endi != src + i) { /* got an integer */
                enode->data.integer =
                    last_read == LAST_READ_SYMBOL ? tmpi * symbol_bit : tmpf;
                enode->type = EXPR_NODE_INT;
                i = endi - src - 1;
            }

            last_read = LAST_READ_NUMBER;
            break;

        case '+':
        case '-':
            while (stac.size != 0) {
                stac_top = stack_top(&stac);
                if (stac_top == NULL)
                    break;
                if (stac_top->data.u8 == '(')
                    break;
                if (stac_top->data.u8 == '+' || stac_top->data.u8 == '-')
                    break;

                stac_node = stack_pop(&stac);
                save_expr_node(&list, enode, list_node, list_tail);
                enode->type = (enum EXPR_NODE_TYPE)stac_node.data.u8;
            }

            stac_node.data.u8 = src[i];

            /* check symbol bit */
            if (last_read == LAST_READ_START ||
                last_read == LAST_READ_BRACKET) {
                symbol_bit = src[i] == '+' ? 1 : -1;
                last_read = LAST_READ_SYMBOL;
            } else {
                stack_push(&stac, stac_node);
                last_read = LAST_READ_OPERATOR;
            }
            break;

        case '*':
        case '/':
            /* check invalid symbol */
            if (last_read == LAST_READ_START || last_read == LAST_READ_BRACKET)
                goto err;
        case '(':
            stac_node.data.u8 = src[i];
            stack_push(&stac, stac_node);
            last_read = src[i] == '(' ? LAST_READ_BRACKET : LAST_READ_OPERATOR;
            break;

        case ')':
            while (stac.size != 0) {
                stac_node = stack_pop(&stac);
                if (stac_node.data.u8 == '(')
                    break;

                save_expr_node(&list, enode, list_node, list_tail);
                enode->type = (enum EXPR_NODE_TYPE)stac_node.data.u8;
            }
            if (stac_node.data.u8 != '(')
                goto err;
            break;

        case ' ':
            break;

        case '\n':
            break;

        case '\0':
            goto end;

        default:
            goto err;
        }
    }

#undef LAST_READ_START
#undef LAST_READ_SYMBOL
#undef LAST_READ_NUMBER
#undef LAST_READ_BRACKET
#undef LAST_READ_OPERATOR

end:
    while (stac.size != 0) {
        stac_node = stack_pop(&stac);
        save_expr_node(&list, enode, list_node, list_tail);
        enode->type = (enum EXPR_NODE_TYPE)stac_node.data.u8;
    }
    stack_free(&stac);
    return list;

#undef save_expr_node

err:
    stack_free(&stac);
    expr_slist_free(&list);
    return list;
}

void expr_slist_free(slist *list)
{
    while (list->head != NULL) {
        free(list->head->data.ptr);
        slist_remove(list, list->head);
    }
    slist_free(list);
}