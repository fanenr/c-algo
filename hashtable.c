#include "hashtable.h"

void
hashtable_move (hashtable_t *table, hashtable_t *origin)
{
  size_t cap = table->cap;
  size_t ocap = origin->cap;
  hashtable_node_t **data = table->data;
  hashtable_node_t **odata = origin->data;

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

  table->size = origin->size;
}

void
hashtable_insert (hashtable_t *table, size_t hash, hashtable_node_t *node)
{
  node->hash = hash;
  node->next = NULL;

  hashtable_node_t **data = table->data;

  size_t index = hash % table->cap;
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
  table->size++;
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

void
hashtable_free (hashtable_t *table, hashtable_dtor_t *dtor)
{
  size_t cap = table->cap;
  hashtable_node_t **data = table->data;

  for (size_t i = 0; i < cap; i++)
    {
      hashtable_node_t *next;
      hashtable_node_t *curr = data[i];

      for (; curr; curr = next)
        {
          next = curr->next;
          dtor (curr);
        }
    }
}
