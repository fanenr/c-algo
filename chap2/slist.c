#include "slist.h"
#include <stdlib.h>

slist *
slist_init (slist *list)
{
  if (list == NULL)
    return NULL;

  list->head = NULL;
  list->size = 0;
  return list;
}

void
slist_free (slist *list)
{
  if (list == NULL)
    return;

  struct slist_n *node = list->head, *next;
  for (; node != NULL; node = next)
    {
      next = node->next;
      free (node);
    }

  slist_init (list);
  return;
}

struct slist_n *
slist_node (void)
{
  struct slist_n *node = malloc (sizeof (struct slist_n));
  if (node == NULL)
    return NULL;

  node->data.ptr = node->next = NULL;
  return node;
}

struct slist_n *
slist_find_from (slist *restrict list, struct slist_n *restrict spos,
                 struct slist_v data)
{
  if (list == NULL || spos == NULL)
    return NULL;

  struct slist_n *find = spos;
  while (find != NULL)
    {
      if (find->data.u64 == data.u64)
        return find;
      find = find->next;
    }

  return NULL;
}

struct slist_n *
slist_find (slist *list, struct slist_v data)
{
  if (list == NULL || list->size == 0)
    return NULL;

  return slist_find_from (list, list->head, data);
}

struct slist_n *
slist_insert_after (slist *restrict list, struct slist_n *restrict pos,
                    struct slist_v data)
{
  if (list == NULL || (pos == NULL && list->size != 0))
    return NULL;

  struct slist_n *node = slist_node ();
  if (node == NULL)
    return NULL;

  node->data = data;
  if (pos != NULL)
    {
      node->next = pos->next;
      pos->next = node;
    }
  else
    {
      list->head = node;
    }

  list->size++;
  return node;
}

struct slist_n *
slist_push_front (slist *restrict list, struct slist_v data)
{
  if (list == NULL)
    return NULL;

  struct slist_n *node = slist_node ();
  if (node == NULL)
    return NULL;

  node->next = list->head;
  node->data = data;
  list->head = node;

  list->size++;
  return node;
}

struct slist_n *
slist_erase (slist *restrict list, struct slist_n *restrict pos)
{
  if (list == NULL || pos == NULL || list->size == 0)
    return NULL;

  struct slist_n *prev, *curr, *next;
  prev = NULL, curr = list->head;

  while (curr != pos && curr != NULL)
    {
      prev = curr;
      curr = curr->next;
    }

  /* did not find pos */
  if (curr == NULL)
    return NULL;

  next = curr->next;
  free (curr);

  if (prev == NULL)
    list->head = next;
  else
    prev->next = next;

  list->size--;
  return pos;
}

void
slist_remove_from (slist *restrict list, struct slist_n *restrict spos,
                   struct slist_v data)
{
  if (list == NULL || spos == NULL || list->size == 0)
    return;

  struct slist_n *prev, *curr, *next;
  prev = NULL, curr = list->head;

  while (curr != NULL)
    {
      if (curr->data.u64 == data.u64)
        break;
      prev = curr;
      curr = curr->next;
    }

  if (curr == NULL)
    return;

  next = curr->next;
  free (curr);

  if (prev == NULL)
    list->head = next;
  else
    prev->next = next;

  list->size--;
  return;
}

void
slist_remove (slist *restrict list, struct slist_v data)
{
  if (list == NULL || list->size == 0)
    return;

  slist_remove_from (list, list->head, data);
  return;
}

void
slist_pop_front (slist *list)
{
  if (list == NULL || list->size == 0)
    return;

  slist_erase (list, list->head);
  return;
}
