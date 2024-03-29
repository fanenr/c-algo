#include "hashtable_ext.h"

void
hashtable_rehash (hashtable_node_t **buckets, size_t cap, hashtable_t *old)
{
  size_t ocap = old->cap;
  hashtable_node_t **odata = old->buckets;

  for (size_t i = 0; i < ocap; i++)
    {
      hashtable_node_t *curr = *odata++;

      for (hashtable_node_t *next; curr; curr = next)
        {
          next = curr->next;
          hashtable_node_t **bucket = buckets + curr->hash % cap;
          hashtable_node_t *head = *bucket;

          *bucket = curr;
          curr->next = head;
        }
    }
}

void
hashtable_insert (hashtable_t *ht, hashtable_node_t *node)
{
  hashtable_node_t **bucket = ht->buckets + node->hash % ht->cap;
  hashtable_node_t *head = *bucket;

  *bucket = node;
  node->next = head;

  ht->size++;
}

void
hashtable_erase (hashtable_t *ht, hashtable_node_t *node)
{
  hashtable_node_t *next = node->next;

  size_t index = node->hash % ht->cap;
  hashtable_node_t **head = ht->buckets + index;
  hashtable_node_t *prev = *head;

  if (node == prev)
    *head = node->next;
  else
    {
      for (hashtable_node_t *temp; node != (temp = prev->next);)
        prev = temp;
      prev->next = node->next;
    }

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
  hashtable_node_t *head = hashtable_head (ht, hash);

  for (hashtable_node_t *curr = head; curr; curr = curr->next)
    if (hash == curr->hash && comp (target, curr) == 0)
      return curr;

  return NULL;
}

void
hashtable_for_each (hashtable_t *ht, hashtable_visit_t *visit)
{
  size_t cap = ht->cap;
  hashtable_node_t **data = ht->buckets;

  for (size_t i = 0; i < cap; i++)
    {
      hashtable_node_t *next;
      hashtable_node_t *curr = data[i];

      for (; curr; curr = next)
        {
          next = curr->next;
          visit (curr);
        }
    }
}
