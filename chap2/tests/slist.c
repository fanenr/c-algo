#include "../slist.h"
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
    slist list1;
    slist_init(&list1);

    assert(list1.size == 0);
    assert(list1.head == NULL);
}

static void test_free(void)
{
    /* free an empty slist */
    slist list1;
    slist_init(&list1);

    slist_free(&list1);

    assert(list1.size == 0);
    assert(list1.head == NULL);

    /* free a slist with some nodes */
    slist list2;
    slist_init(&list2);

    struct slist_n *node, *tail;
    tail = NULL;

    for (int i = 0; i < 100; i++) {
        node = slist_node();
        node->data.i32 = i;
        tail = slist_insert(&list2, tail, node);
    }

    slist_free(&list2);

    assert(list2.size == 0);
    assert(list2.head == NULL);
}

static void test_node(void)
{
    struct slist_n *node;
    node = slist_node();

    assert(node->next == NULL);
    assert(node->data.ptr == NULL);

    free(node);
}

static void test_insert(void)
{
    slist list1;
    slist_init(&list1);

    /* tail insertion */
    struct slist_n *node, *tail, *head;
    tail = head = NULL;

    for (int i = 0; i < 1000; i++) {
        node = slist_node();
        node->data.i32 = i;
        tail = slist_insert(&list1, tail, node);

        assert(list1.size == i + 1);
    }

    slist_free(&list1);

    /* head insertion */
    slist list2;
    slist_init(&list2);

    for (int i = 0; i < 1000; i++) {
        node = slist_node();
        node->data.i32 = i;
        head = slist_insert(&list2, NULL, node);

        assert(node == head);
        assert(list2.head == head);
        assert(list2.size == i + 1);
    }

    slist_free(&list2);
}

static void test_remove(void)
{
    slist list1;
    slist_init(&list1);

    /* remove NULL */
    slist_remove(&list1, NULL);

    assert(list1.size == 0);
    assert(list1.head == NULL);

    struct slist_n *node, *tail;
    tail = NULL;

    for (int i = 0; i < 1000; i++) {
        node = slist_node();
        node->data.i32 = i;
        tail = slist_insert(&list1, tail, node);
    }

    for (int i = 0; i < 1000; i++) {
        assert(list1.head->data.i32 == i);

        slist_remove(&list1, list1.head);

        assert(list1.size == 999 - i);
    }

    assert(list1.size == 0);
    assert(list1.head == NULL);

    slist_free(&list1);
}