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

#define save_expr_node(plist, enode, lnode, ltail)                             \
    enode = alloc_expr_node();                                                 \
    lnode = slist_node();                                                      \
    lnode->data.ptr = enode;                                                   \
    ltail = slist_insert(plist, ltail, lnode);

slist in2post(char *src, size_t len)
{
    stack stac;
    slist list;
    stack_init(&stac);
    slist_init(&list);

    struct stack_n stac_node, *stac_top;
    struct slist_n *list_node, *list_tail;
    list_tail = NULL;
    stac_node.data.u8 = 0;

    long tmpi;
    double tmpf;
    char *endi, *endf;
    struct expr_node *enode;

    for (int i = 0; i < len; i++) {
        switch (src[i]) {
        case '0' ... '9':
            endi = endf = src + i;
            tmpf = strtod(src + i, &endf);
            tmpi = strtol(src + i, &endi, 10);

            save_expr_node(&list, enode, list_node, list_tail);

            if (endf != src + i) { /* got a floating */
                enode->data.floating = tmpf;
                enode->type = EXPR_NODE_FLOAT;
            } else if (endi != src + i) { /* got an integer */
                enode->data.integer = tmpi;
                enode->type = EXPR_NODE_INT;
            } else {
                goto err;
            }

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
            stack_push(&stac, stac_node);
            break;

        case '*':
        case '/':
        case '(':
            stac_node.data.u8 = src[i];
            stack_push(&stac, stac_node);
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

        case '\0':
            goto end;

        default:
            goto err;
        }
    }

end:
    while (stac.size != 0) {
        stac_node = stack_pop(&stac);

        save_expr_node(&list, enode, list_node, list_tail);

        enode->type = (enum EXPR_NODE_TYPE)stac_node.data.u8;
    }
    stack_free(&stac);
    return list;

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