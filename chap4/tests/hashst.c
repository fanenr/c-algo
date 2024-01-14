#include "../hashst.h"
#include <assert.h>
#include <string.h>

static void test_init (void);
static void test_reserve (void);
static void test_free (void);
static void test_hash (void);
static void test_find (void);
static void test_insert (void);
static void test_remove (void);

#define build_val(_val_t, _val)                                               \
  ({                                                                          \
    union hashst_v _ret;                                                      \
    _ret._val_t = _val;                                                       \
    _ret;                                                                     \
  })

int
main (void)
{
  test_init ();
  test_reserve ();
  test_free ();
  test_hash ();
  test_find ();
  test_insert ();
  test_remove ();

  return 0;
}

static void
test_init (void)
{
  hashst set1;
  hashst_init (&set1, HASHST_VAL_INT);

  assert (set1.size == 0);
  assert (set1.head == NULL);
  assert (set1.capacity == 0);
  assert (set1.vtype == HASHST_VAL_INT);

  return;
}

static void
test_reserve (void)
{
  hashst set1;
  hashst_init (&set1, HASHST_VAL_INT);

  hashst_reserve (&set1, 8);

  assert (set1.head != NULL);
  assert (set1.capacity == 8);

  hashst_reserve (&set1, 100);

  assert (set1.head != NULL);
  assert (set1.capacity == 100);

  hashst_free (&set1);

  return;
}

static void
test_free (void)
{
  hashst set1;
  hashst_init (&set1, HASHST_VAL_INT);

  hashst_free (&set1);

  assert (set1.size == 0);
  assert (set1.head == NULL);
  assert (set1.capacity == 0);
  assert (set1.vtype == HASHST_VAL_INT);

  hashst set2;
  hashst_init (&set2, HASHST_VAL_INT);

  for (int i = 0; i < 1000; i++)
    hashst_insert (&set2, build_val (i64, i));

  hashst_free (&set2);

  assert (set2.size == 0);
  assert (set2.head == NULL);
  assert (set2.capacity == 0);
  assert (set2.vtype == HASHST_VAL_INT);

  return;
}

static size_t
hashmp_hash_sdbm (const char *str)
{
  size_t hash = 0;
  for (size_t i = 0; str[i] != 0; i++)
    hash = str[i] + (hash << 6) + (hash << 16) - hash;
  return hash;
}

static void
test_hash (void)
{
  /* test i64 val hash */
  hashst set1;
  hashst_init (&set1, HASHST_VAL_INT);

  /* capacity is 0 */
  assert (hashst_hash (&set1, build_val (i64, 10)) == -1);

  hashst_reserve (&set1, HASHST_INIT_CAP);

  assert (hashst_hash (&set1, build_val (i64, 10)) == 10 % 8);

  hashst_free (&set1);

  /* test f64 val hash */
  hashst set2;
  hashst_init (&set2, HASHST_VAL_FLT);

  /* capacity is 0 */
  assert (hashst_hash (&set2, build_val (f64, 3.14)) == -1);

  hashst_reserve (&set2, HASHST_INIT_CAP);

  double key_f64 = 3.14;
  assert (hashst_hash (&set2, build_val (f64, 3.14))
          == *(int64_t *)&key_f64 % 8);

  hashst_free (&set2);

  /* test str val hash */
  hashst set3;
  hashst_init (&set3, HASHST_VAL_STR);

  /* capacity is 0 */
  assert (hashst_hash (&set3, build_val (str, "Hello World")) == -1);

  hashst_reserve (&set3, HASHST_INIT_CAP);

  assert ((size_t)hashst_hash (&set3, build_val (str, "Hello World"))
          == hashmp_hash_sdbm ("Hello World") % 8);

  hashst_free (&set3);

  return;
}

static void
test_find (void)
{
  hashst set1;
  hashst_init (&set1, HASHST_VAL_STR);

  hashst_insert (&set1, build_val (str, "Tom"));
  hashst_insert (&set1, build_val (str, "John"));

  union hashst_v *val;
  val = hashst_find (&set1, build_val (str, "Tom"));
  assert (strcmp (val->str, "Tom") == 0);

  val = hashst_find (&set1, build_val (str, "John"));
  assert (strcmp (val->str, "John") == 0);

  val = hashst_find (&set1, build_val (str, "Mike"));
  assert (val == NULL);

  val = hashst_find (&set1, build_val (str, NULL));
  assert (val->str == NULL);

  hashst_free (&set1);

  return;
}

static void
test_insert (void)
{
  hashst set1;
  hashst_init (&set1, HASHST_VAL_STR);

  assert (hashst_insert (&set1, build_val (str, "Tom")) != NULL);
  assert (set1.size == 1);

  assert (hashst_insert (&set1, build_val (str, "John")) != NULL);
  assert (set1.size == 2);

  assert (hashst_insert (&set1, build_val (str, NULL)) != NULL);
  assert (set1.size == 3);

  hashst_free (&set1);

  return;
}

static void
test_remove (void)
{
  hashst set1;
  hashst_init (&set1, HASHST_VAL_STR);

  hashst_insert (&set1, build_val (str, "Tom"));
  hashst_insert (&set1, build_val (str, "John"));

  hashst_remove (&set1, build_val (str, "Tom"));

  assert (set1.size == 1);
  assert (hashst_find (&set1, build_val (str, "Tom")) == NULL);

  hashst_remove (&set1, build_val (str, "John"));

  assert (set1.size == 0);
  assert (hashst_find (&set1, build_val (str, "John")) == NULL);

  hashst_free (&set1);

  return;
}
