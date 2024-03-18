#include "../list.h"
#include <assert.h>

LIST_DEF_ALL (i32, int);

int
main (void)
{
  i32_list lis;
  i32_list_init (&lis);

  for (int i = 0; i < 1000; i++)
    assert (i32_list_push_back (&lis, i));

  assert (lis.size == 1000);

  i32_list_n *node = i32_list_at (&lis, 0);
  for (int i = 0; i < 1000; i++)
    {
      assert (node->data == i);
      node = node->next;
    }

  i32_list_free (&lis);
}
