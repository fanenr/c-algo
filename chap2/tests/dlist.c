#include "../dlist.h"
#include <assert.h>
#include <stdlib.h>

static void test_init(void);
static void test_free(void);
static void test_node(void);
static void test_insert(void);
static void test_remove(void);

int main(void)
{
    test_init();
    test_free();
    test_node();
    test_insert();
    test_remove();
}

static void test_init(void)
{
    dlist list1;
    dlist_init(&list1);

    assert(list1.size == 0);
    assert(list1.head == NULL);
}

static void test_free(void)
{
    /* free a empty dlist */
    dlist list1;
    dlist_init(&list1);

    dlist_free(&list1);

    assert(list1.size == 0);
    assert(list1.head == NULL);

    /* free a dlist with some nodes */
    dlist list2;
    dlist_init(&list2);

    struct dlist_n *node, *tail;
    tail = NULL;

    for (int i = 0; i < 100; i++) {
        node = dlist_node();
        node->data.i32 = i;
        tail = dlist_insert(&list2, tail, node);
    }

    dlist_free(&list2);

    assert(list2.size == 0);
    assert(list2.head == NULL);
}

static void test_node(void)
{
    struct dlist_n *node;
    node = dlist_node();

    assert(node->prev == NULL);
    assert(node->next == NULL);
    assert(node->data.ptr == NULL);

    free(node);
}

static void test_insert(void)
{
    dlist list1;
    dlist_init(&list1);

    /* tail insertion */
    struct dlist_n *node, *tail, *head;
    tail = head = NULL;

    for (int i = 0; i < 1000; i++) {
        node = dlist_node();
        node->data.i32 = i;
        tail = dlist_insert(&list1, tail, node);

        assert(list1.size == i + 1);
    }

    dlist_free(&list1);

    /* head insertion */
    dlist list2;
    dlist_init(&list2);

    for (int i = 0; i < 1000; i++) {
        node = dlist_node();
        node->data.i32 = i;
        head = dlist_insert(&list2, NULL, node);

        assert(node == head);
        assert(list2.head == head);
        assert(list2.size == i + 1);
    }

    dlist_free(&list2);
}

static void test_remove(void)
{
    dlist list1;
    dlist_init(&list1);

    /* remove NULL */
    dlist_remove(&list1, NULL);

    assert(list1.size == 0);
    assert(list1.head == NULL);

    struct dlist_n *node, *tail;
    tail = NULL;

    for (int i = 0; i < 1000; i++) {
        node = dlist_node();
        node->data.i32 = i;
        tail = dlist_insert(&list1, tail, node);
    }

    for (int i = 0; i < 1000; i++) {
        assert(list1.head->data.i32 == i);

        dlist_remove(&list1, list1.head);

        assert(list1.size == 999 - i);
    }

    assert(list1.size == 0);
    assert(list1.head == NULL);

    dlist_free(&list1);
}