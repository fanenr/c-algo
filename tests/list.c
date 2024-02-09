#include "../list.h"
#include <assert.h>

LIST_DEFS (int, i32);

int
main (void)
{
  list lis;
  list_init (&lis);

  for (int i = 0; i < 1000; i++)
    assert (i32_list_push_back (&lis, i));

  assert (lis.len == 1000);

  i32_list_n *node = i32_list_at (&lis, 0);
  for (int i = 0; i < 1000; i++)
    {
      assert (*node->data == i);
      node = node->next;
    }

  list_free (&lis);
}
