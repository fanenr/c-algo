#include "list.h"

void
list_init (list_t *list, list_comp_t comp)
{
  *list = (list_t){ .comp_fn = comp };
}

void
list_remove (list_t *list, list_node_t *node)
{
  list_node_t *prev = node->prev;
  list_node_t *next = node->next;

  if (node == list->head)
    list->head = next;
  if (node == list->tail)
    list->tail = prev;

  if (prev)
    prev->next = next;
  if (next)
    next->prev = prev;

  list->size--;
}

list_node_t *
list_at (const list_t *list, size_t index)
{
  if (index >= list->size)
    return NULL;

  list_node_t *ret = list->head;
  for (; index; index--)
    ret = ret->next;

  return ret;
}

list_node_t *
list_push_back (list_t *list, list_node_t *node)
{
  node->next = NULL;
  node->prev = list->tail;

  if (list->tail)
    list->tail->next = node;
  else
    list->head = node;
  list->tail = node;

  list->size++;
  return node;
}

list_node_t *
list_push_front (list_t *list, list_node_t *node)
{
  node->prev = NULL;
  node->next = list->head;

  if (list->head)
    list->head->prev = node;
  else
    list->tail = node;
  list->head = node;

  list->size++;
  return node;
}

list_node_t *
list_insert_front (list_t *list, list_node_t *pos, list_node_t *node)
{
  node->next = pos;
  node->prev = pos->prev;

  if (pos->prev)
    pos->prev->next = node;
  else
    list->head = node;
  pos->prev = node;

  list->size++;
  return node;
}

list_node_t *
list_insert_back (list_t *list, list_node_t *pos, list_node_t *node)
{
  node->prev = pos;
  node->next = pos->next;

  if (pos->next)
    pos->next->prev = node;
  else
    list->tail = node;
  pos->next = node;

  list->size++;
  return node;
}

list_node_t *
list_insert_at (list_t *list, size_t index, list_node_t *node)
{
  if (index > list->size)
    return NULL;

  if (index == list->size)
    return list_push_back (list, node);

  if (index == 0)
    return list_push_front (list, node);

  return list_insert_front (list, list_at (list, index), node);
}
