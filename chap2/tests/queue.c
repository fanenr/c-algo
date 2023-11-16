#include "../queue.h"
#include <assert.h>
#include <stdlib.h>

static void test_init(void);
static void test_free(void);
static void test_node(void);
static void test_push(void);
static void test_top(void);
static void test_pop(void);

int main(void)
{
    test_init();
    test_free();
    test_node();
    test_push();
    test_top();
    test_pop();
}

static void test_init(void)
{
    queue que1;
    queue_init(&que1);

    assert(que1.size == 0);
    assert(que1.head == NULL);
    assert(que1.tail == NULL);
}

static void test_free(void)
{
    /* free a empty queue */
    queue que1;
    queue_init(&que1);

    queue_free(&que1);

    assert(que1.size == 0);
    assert(que1.head == NULL);
    assert(que1.tail == NULL);

    /* free a queue with some nodes */
    queue que2;
    queue_init(&que2);

    struct queue_n *node;
    for (int i = 0; i < 100; i++) {
        node = queue_node();
        node->data.i32 = i;
        queue_push(&que2, node);
    }

    queue_free(&que2);

    assert(que1.size == 0);
    assert(que1.head == NULL);
    assert(que1.tail == NULL);
}

static void test_node(void)
{
    struct queue_n *node;
    node = queue_node();

    assert(node->next == NULL);
    assert(node->data.ptr == NULL);

    free(node);
}

static void test_push(void)
{
    /* try to push some nodes into deque */
    queue que1;
    queue_init(&que1);

    struct queue_n *node, *ptr, *head;

    node = queue_node();
    queue_push(&que1, node);
    head = que1.head;

    assert(que1.size == 1);
    assert(que1.head == node);
    assert(que1.tail == node);

    for (int i = 1; i < 1000; i++) {
        node = queue_node();
        node->data.i32 = i;
        ptr = queue_push(&que1, node);

        assert(ptr == node);
        assert(que1.tail == ptr);
        assert(que1.head == head);
        assert(que1.size == i + 1);
        assert(ptr->data.i32 == node->data.i32);
    }

    queue_free(&que1);
}

static void test_top(void)
{
    /* NULL will be returned if queue is empty */
    queue que1;
    queue_init(&que1);

    assert(queue_top(&que1) == NULL);

    /* pointer of top node will be returned if queue has nodes */
    queue que2;
    queue_init(&que2);

    struct queue_n *node;
    for (int i = 0; i < 1000; i++) {
        node = queue_node();
        node->data.i32 = i;
        queue_push(&que2, node);

        assert(queue_top(&que2)->data.i32 == 0);
    }

    queue_free(&que2);
}

static void test_pop(void)
{
    /* a empty queue_n will be returned if queue is empty */
    queue que1;
    queue_init(&que1);

    assert(queue_pop(&que1).data.ptr == NULL);
    assert(que1.head == NULL);
    assert(que1.tail == NULL);
    assert(que1.size == 0);

    /* a copy of the top node will be returned if queue has nodes */
    queue que2;
    queue_init(&que2);

    struct queue_n *node, copy;
    for (int i = 0; i < 1000; i++) {
        node = queue_node();
        node->data.i32 = i;
        queue_push(&que2, node);
    }

    for (int i = 0; i < 1000; i++) {
        copy = queue_pop(&que2);

        assert(copy.next == NULL);
        assert(copy.data.i32 == i);
        assert(que2.size == 999 - i);
    }

    assert(que2.size == 0);
    assert(que2.head == NULL);
    assert(que2.tail == NULL);

    queue_free(&que2);
}