#include "hashtable.h"

#define likely(exp) __builtin_expect (!!(exp), 1)
#define unlikely(exp) __builtin_expect (!!(exp), 0)

void
hashtable_rehash (hashtable_node_t **data, size_t cap, hashtable_t *ht)
{
  size_t ht_cap = ht->cap;
  hashtable_node_t **ht_data = ht->data;

  for (size_t i = 0; i < ht_cap; i++)
    for (hashtable_node_t **head, *next, *curr = *ht_data++; curr; curr = next)
      {
	next = curr->next;
	head = data + curr->hash % cap;
	if ((curr->next = *head))
	  (*head)->prev = curr;
	curr->prev = NULL;
	*head = curr;
      }
}

void
hashtable_link (hashtable_t *ht, hashtable_node_t **inpos,
		hashtable_node_t *prev, hashtable_node_t *node)
{
  hashtable_node_t *next = *inpos;
  node->prev = prev;
  node->next = next;
  *inpos = node;

  if (prev)
    prev->next = node;
  if (next)
    next->prev = node;

  ht->size++;
}

void
hashtable_erase (hashtable_t *ht, hashtable_node_t *node)
{
  hashtable_node_t *prev = node->prev;
  hashtable_node_t *next = node->next;

  if (next)
    next->prev = prev;
  if (prev)
    prev->next = next;
  else
    ht->data[node->hash % ht->cap] = next;

  ht->size--;
}

/* **************************************************************** */
/*                               ext                                */
/* **************************************************************** */

hashtable_node_t *
hashtable_find (const hashtable_t *ht, const hashtable_node_t *target,
		hashtable_comp_t *comp)
{
  size_t hash = target->hash;

  for (hashtable_node_t *curr = ht->data[hash % ht->cap]; curr;
       curr = curr->next)
    if (hash == curr->hash && comp (target, curr) == 0)
      return curr;

  return NULL;
}

hashtable_node_t *
hashtable_insert (hashtable_t *ht, hashtable_node_t *node,
		  hashtable_comp_t *comp)
{
  size_t hash = node->hash;
  hashtable_node_t *prev = NULL;
  hashtable_node_t **head = ht->data + hash % ht->cap;

  for (hashtable_node_t *curr = *head; curr; curr = curr->next)
    {
      if (unlikely (hash == curr->hash && comp (node, curr) == 0))
	return NULL;
      prev = curr;
    }

  hashtable_node_t **inpos = prev ? &prev->next : head;
  hashtable_link (ht, inpos, prev, node);
  return node;
}

void
hashtable_visit (hashtable_t *ht, hashtable_visit_t *visit)
{
  size_t cap = ht->cap;
  hashtable_node_t **data = ht->data;

  for (size_t i = 0; i < cap; i++)
    for (hashtable_node_t *next, *curr = *data++; curr; curr = next)
      {
	next = curr->next;
	visit (curr);
      }
}
