#include "hashtable.h"

void
hashtable_rehash (hashtable_node_t **slots, size_t cap, hashtable_t *ht)
{
  size_t ht_cap = ht->cap;
  hashtable_node_t **ht_head = ht->slots;

  for (size_t i = 0; i < ht_cap; i++)
    {
      hashtable_node_t *curr = *ht_head++;

      for (hashtable_node_t *next; curr; curr = next)
        {
          next = curr->next;
          hashtable_node_t **head = slots + curr->hash % cap;
          hashtable_node_t *temp = *head;
          curr->next = temp;
          curr->prev = NULL;
          *head = curr;
          if (temp)
            temp->prev = curr;
        }
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
  hashtable_node_t **head = ht->slots + node->hash % ht->cap;
  hashtable_node_t *prev = node->prev;
  hashtable_node_t *next = node->next;

  if (!prev)
    *head = next;
  else
    prev->next = next;

  if (next)
    next->prev = prev;

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
  hashtable_node_t *curr = hashtable_head (ht, hash);

  for (; curr; curr = curr->next)
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
  hashtable_node_t **head = ht->slots + hash % ht->cap;

  for (hashtable_node_t *curr = *head; curr; curr = curr->next)
    {
      if (gcc_unlikely (hash == curr->hash && comp (node, curr) == 0))
        return NULL;
      prev = curr;
    }

  hashtable_node_t **inpos = prev ? &prev->next : head;
  hashtable_link (ht, inpos, prev, node);
  return node;
}

void
hashtable_for_each (hashtable_t *ht, hashtable_visit_t *visit)
{
  size_t cap = ht->cap;
  hashtable_node_t **head = ht->slots;

  for (size_t i = 0; i < cap; i++)
    {
      hashtable_node_t *curr = *head++;

      for (hashtable_node_t *next; curr; curr = next)
        {
          next = curr->next;
          visit (curr);
        }
    }
}
