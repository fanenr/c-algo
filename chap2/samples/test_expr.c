#include "expr.h"
#include <assert.h>
#include <stdio.h>

#define BUFF_SIZE 1024

static void test_in2post();

int main(void)
{
    test_in2post();
}

void test_in2post()
{
    slist list;
    char buf[BUFF_SIZE];

    scanf("%s", buf);
    list = in2post(buf, BUFF_SIZE);

    assert(list.size != 0);

    struct slist_n *node;
    struct expr_node *enode;

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

    expr_slist_free(&list);
}