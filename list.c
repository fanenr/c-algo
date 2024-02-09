#include "list.h"
#include <stdlib.h>
#include <string.h>

void
list_init (list *lis)
{
  *lis = (list){ .len = 0, .head = NULL, .tail = NULL };
}

void
list_free (list *lis)
{
  list_n *node = lis->head, *next;
  for (; node != NULL; node = next)
    {
      next = node->next;
      free (node);
    }

  list_init (lis);
}

list_n *
list_at (list *lis, size_t pos)
{
  if (pos >= lis->len)
    return NULL;

  list_n *ret = lis->head;
  for (; pos; pos--)
    ret = ret->next;

  if (!pos)
    return ret;
  return NULL;
}

list_n *
list_remove (list *lis, list_n *pos)
{
  list_n *prev, *curr, *next;
  prev = pos->prev, curr = pos, next = pos->next;

  free (curr);

  if (lis->len == 1)
    lis->head = lis->tail = NULL;
  else if (pos == lis->head)
    lis->head = next;
  else if (pos == lis->tail)
    lis->tail = prev;
  else
    {
      prev->next = next;
      next->prev = prev;
    }

  lis->len--;
  return next;
}

list_n *
list_push_back (list *lis, void *data, size_t ele)
{
  list_n *node = malloc (sizeof (list_n) + ele);
  if (!node)
    return NULL;

  node->next = NULL;
  node->prev = lis->tail;
  if (memcpy (node->data, data, ele) != node->data)
    return NULL;

  lis->tail = node;
  if (!lis->len)
    lis->head = node;

  lis->len++;
  return node;
}

list_n *
list_push_front (list *lis, void *data, size_t ele)
{
  list_n *node = malloc (sizeof (list_n) + ele);
  if (!node)
    return NULL;

  node->prev = NULL;
  node->next = lis->head;
  if (memcpy (node->data, data, ele) != node->data)
    return NULL;

  lis->head = node;
  if (!lis->len)
    lis->tail = node;

  lis->len++;
  return node;
}

list_n *
list_insert (list *lis, list_n *pos, void *data, size_t ele)
{
  list_n *node = malloc (sizeof (list_n) + ele);
  if (!node)
    return NULL;

  node->next = pos;
  node->prev = pos->prev;
  if (memcpy (node->data, data, ele) != node->data)
    return NULL;

  if (pos != lis->head)
    pos->prev->next = node;
  else
    lis->head = node;
  pos->prev = node;

  lis->len++;
  return node;
}
