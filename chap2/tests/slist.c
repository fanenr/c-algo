#include "../slist.h"
#include <assert.h>
#include <stdlib.h>

int main()
{
    slist list;

    slist_init(&list);
    assert(list.head == NULL && list.size == 0);

    int val = 10;
    struct slist_n *node = slist_node();
    node->value = &val;
    assert(slist_insert(&list, NULL, node) == node);
    assert(list.head == node && list.size == 1);
    assert(list.head->value == &val);

    int *val2 = malloc(sizeof(int));
    struct slist_n *node2 = slist_node();
    node2->value = val2;
    assert(slist_insert(&list, node, node2) == node2);
    assert(list.head == node && list.size == 2);
    assert(list.head->next == node2);

    slist_remove(&list, node);
    assert(list.head == node2 && list.size == 1);

    slist_remove(&list, node2);
    assert(list.head == NULL && list.size == 0);

    slist_free(&list);
    free(val2);
}