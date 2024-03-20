#include "../list.h"
#include <assert.h>
#include <stdlib.h>

typedef struct data data;

struct data
{
  list_node_t node;
  int num;
};

static list_node_t *data_list_node_new (int num);
static void data_list_free (list_t *list);

int
main (void)
{
}

static list_node_t *
data_list_node_new (int num)
{
  data *node = malloc (sizeof (data));
  assert (node);

  node->node = list_node_init_v;
  node->num = num;

  return &node->node;
}
