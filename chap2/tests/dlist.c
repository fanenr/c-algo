#include "../dlist.h"
#include <assert.h>
#include <stdlib.h>

int main()
{
    dlist list;

    dlist_init(&list);
    assert(list.head == NULL && list.size == 0);

    struct dlist_n *node = dlist_node();
    node->data.i32 = 10;
    assert(dlist_insert(&list, NULL, node) == node);
    assert(list.head == node && list.size == 1);
    assert(node->prev == NULL && node->next == NULL);
    assert(node->data.i32 == 10);

    int *val2 = malloc(sizeof(int));
    struct dlist_n *node2 = dlist_node();
    node2->data.ptr = val2;
    assert(dlist_insert(&list, node, node2) == node2);
    assert(list.head == node && list.size == 2);
    assert(list.head->next == node2);
    assert(node->next == node2 && node2->prev == node && node2->next == NULL);

    struct dlist_n *node3 = dlist_node();
    dlist_insert(&list, node, node3);
    assert(list.head == node && list.size == 3);
    assert(node->next == node3 && node3->prev == node);
    assert(node3->next == node2 && node2->prev == node3 && node2->next == NULL);

    dlist_remove(&list, node);
    assert(list.head == node3 && list.size == 2);
    assert(node3->prev == NULL && node3->next == node2);

    dlist_free(&list);
    free(val2);
}