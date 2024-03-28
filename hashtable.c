#include "hashtable_ext.h"

void
hashtable_move (hashtable_node_t **data, size_t cap, hashtable_t *old)
{
  size_t ocap = old->cap;
  hashtable_node_t **odata = old->data;

  for (size_t i = 0; i < ocap; i++)
    {
      hashtable_node_t *next;
      hashtable_node_t *curr = odata[i];

      for (; curr; curr = next)
        {
          next = curr->next;
          size_t index = curr->hash % cap;
          hashtable_node_t **head = data + index;
          hashtable_node_t *tail = *head;

          if (!tail)
            {
              curr->prev = curr->next = NULL;
              *head = curr;
              continue;
            }

          for (hashtable_node_t *temp; (temp = tail->next);)
            tail = temp;

          curr->prev = tail;
          curr->next = NULL;
          tail->next = curr;
        }
    }
}

void
hashtable_insert (hashtable_t *ht, size_t hash, hashtable_node_t *node)
{
  node->hash = hash;
  node->next = NULL;

  hashtable_node_t **data = ht->data;

  size_t index = hash % ht->cap;
  hashtable_node_t **head = data + index;
  hashtable_node_t *tail = *head;

  if (!tail)
    {
      node->prev = NULL;
      *head = node;
      goto inc_size;
    }

  for (hashtable_node_t *temp; (temp = tail->next);)
    tail = temp;

  node->prev = tail;
  tail->next = node;

inc_size:
  ht->size++;
}

void
hashtable_erase (hashtable_t *ht, hashtable_node_t *node)
{
  hashtable_node_t *next = node->next;
  hashtable_node_t *prev = node->prev;

  size_t index = node->hash % ht->cap;
  hashtable_node_t **head = ht->data + index;

  if (node == *head)
    *head = next;

  if (prev)
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
  hashtable_node_t **data = ht->data;

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
