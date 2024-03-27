#include "hashtable.h"

hashtable_t *
hashtable_move (hashtable_t *table, hashtable_t *origin)
{
  size_t cap = table->cap;
  size_t ocap = origin->cap;
  hashtable_node_t **data = table->data;
  hashtable_node_t **odata = origin->data;

  for (size_t i = 0; i < ocap; i++)
    {
      for (hashtable_node_t *curr = odata[i]; curr; curr = curr->next)
        {
          size_t index = curr->hash % cap;
          hashtable_node_t *node = data[index];

          if (!node)
            {
              data[index] = curr;
              continue;
            }

          hashtable_node_t *tail = node;
          for (hashtable_node_t *temp; (temp = tail->next);)
            tail = temp;

          tail->next = curr;
        }
    }

  table->size = origin->size;
  return table;
}

hashtable_node_t *
hashtable_insert (hashtable_t *table, size_t hash, hashtable_node_t *node)
{
  node->hash = hash;
  node->next = NULL;

  hashtable_node_t **data = table->data;

  size_t index = hash % table->cap;
  hashtable_node_t *head = data[index];

  if (!head)
    {
      data[index] = node;
      node->prev = NULL;
      goto inc_size;
    }

  hashtable_node_t *tail = head;
  for (hashtable_node_t *temp; (temp = tail->next);)
    tail = temp;

  node->prev = tail;
  tail->next = node;

inc_size:
  table->size++;
  return node;
}

void
hashtable_erase (hashtable_t *table, hashtable_node_t *node)
{
  hashtable_node_t *next = node->next;
  hashtable_node_t *prev = node->prev;

  size_t index = node->hash % table->cap;
  hashtable_node_t **head = table->data + index;

  if (node == *head)
    *head = next;

  if (prev)
    prev->next = next;
  if (next)
    next->prev = prev;

  table->size--;
}
