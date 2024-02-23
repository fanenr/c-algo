#include "list.h"
#include <stdalign.h>
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
  for (size_t i = lis->len; i; i--)
    {
      next = node->next;
      free (node);
      node = next;
    }

  list_init (lis);
}

void
list_remove (list *lis, list_n *pos)
{
  if (!lis->len)
    return;

  list_n *prev = pos->prev;
  list_n *next = pos->next;
  free (pos);

  if (pos == lis->head)
    lis->head = next;
  if (pos == lis->tail)
    lis->tail = prev;

  if (prev)
    prev->next = next;
  if (next)
    next->prev = prev;

  lis->len--;
}

list_n *
list_at (const list *lis, size_t pos)
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
  list_n *node = malloc (info->n_size);
  if (!node)
    return NULL;

  node->next = NULL;
  node->prev = lis->tail;
  void *val = (void *)node + info->d_offs;
  if (memcpy (val, data, info->d_size) != val)
    return NULL;

  if (lis->tail)
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
  list_n *node = malloc (info->n_size);
  if (!node)
    return NULL;

  node->prev = NULL;
  node->next = lis->head;
  void *val = (void *)node + info->d_offs;
  if (memcpy (val, data, info->d_size) != val)
    return NULL;

  if (lis->head)
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
  list_n *node = malloc (info->n_size);
  if (!node)
    return NULL;

  node->next = pos;
  node->prev = pos->prev;
  void *val = (void *)node + info->d_offs;
  if (memcpy (val, data, info->d_size) != val)
    return NULL;

  if (pos != lis->head)
    pos->prev->next = node;
  else
    lis->head = node;
  pos->prev = node;

  lis->len++;
  return node;
}
