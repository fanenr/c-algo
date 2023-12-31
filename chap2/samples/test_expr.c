#include "expr.h"
#include <assert.h>
#include <stdio.h>

#define BUFF_SIZE 1024

static void test_in2post(void);

int
main(void)
{
    test_in2post();
}

void
test_in2post(void)
{
    slist list;
    char buf[BUFF_SIZE];

    struct slist_n *node;
    struct expr_node *enode, result;

    while (fgets(buf, BUFF_SIZE, stdin) != NULL) {
        printf("infix: %s", buf);
        list = expr_in2post(buf, BUFF_SIZE);
        assert(list.size != 0);
        printf("postfix: ");

        for (node = list.head; node != NULL; node = node->next) {
            enode = node->data.ptr;
            if (enode->type == EXPR_NODE_FLOAT) {
                printf("%.2lf ", enode->data.floating);
            } else if (enode->type == EXPR_NODE_INT) {
                printf("%ld ", enode->data.integer);
            } else {
                printf("%c ", (char)enode->type);
            }
        }

        expr_evaluate(&list, &result);
        assert(result.type != EXPR_NODE_NONE);
        if (result.type == EXPR_NODE_INT)
            printf("= %ld", result.data.integer);
        else
            printf("= %.2lf", result.data.floating);

        printf("\n\n");
        expr_free(&list);
    }
}
