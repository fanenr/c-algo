#include "list.h"
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

static inline size_t
data_offset (const list_i *info)
{
  if (alignof (list_n) >= info->align)
    return 0;

  return info->align - alignof (list_n);
}

static inline size_t
data_size (const list_i *info)
{
  return info->size + data_offset (info);
}

static inline void *
data_ptr (list_n *node, const list_i *info)
{
  return (void *)node + sizeof (list_n) + data_offset (info);
}

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

void
list_remove (list *lis, list_n *pos)
{
  list_n *prev, *curr, *next;
  prev = pos->prev, next = pos->next;

  free (pos);

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
}

list_n *
list_at (list *lis, size_t pos)
{
  if (pos >= lis->len)
    return NULL;

  list_n *ret = lis->head;
  for (; pos; pos--)
    ret = ret->next;

  return ret;
}

list_n *
list_push_back (list *lis, void *data, const list_i *info)
{
  size_t size = data_size (info);
  list_n *node = malloc (sizeof (list_n) + size);
  if (!node)
    return NULL;

  node->next = NULL;
  node->prev = lis->tail;
  node->data = data_ptr (node, info);
  if (memcpy (node->data, data, info->size) != node->data)
    return NULL;

  if (lis->len)
    lis->tail->next = node;
  else
    lis->head = node;
  lis->tail = node;

  lis->len++;
  return node;
}

list_n *
list_push_front (list *lis, void *data, const list_i *info)
{
  size_t size = data_size (info);
  list_n *node = malloc (sizeof (list_n) + size);
  if (!node)
    return NULL;

  node->prev = NULL;
  node->next = lis->head;
  node->data = data_ptr (node, info);
  if (memcpy (node->data, data, info->size) != node->data)
    return NULL;

  if (lis->len)
    lis->head->prev = node;
  else
    lis->tail = node;
  lis->head = node;

  lis->len++;
  return node;
}

list_n *
list_insert (list *lis, list_n *pos, void *data, const list_i *info)
{
  size_t size = data_size (info);
  list_n *node = malloc (sizeof (list_n) + size);
  if (!node)
    return NULL;

  node->next = pos;
  node->prev = pos->prev;
  node->data = data_ptr (node, info);
  if (memcpy (node->data, data, info->size) != node->data)
    return NULL;

  if (pos != lis->head)
    pos->prev->next = node;
  else
    lis->head = node;
  pos->prev = node;

  lis->len++;
  return node;
}
