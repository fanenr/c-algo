#include "avltree.h"

#define HEIGHT_MAX 36

#define HEIGHT_OF(NODE) ((NODE) ? (NODE)->height : -1)
#define BALANCE_FACTOR_OF(NODE)                                               \
  ((NODE) ? HEIGHT_OF ((NODE)->left) - HEIGHT_OF ((NODE)->right) : 0)

void
avltree_init (avltree_t *tree, avltree_comp_t *comp)
{
  *tree = (avltree_t){ .comp_fn = comp };
}

static inline avltree_height_t
height_update (avltree_node_t *node)
{
  avltree_height_t lh = HEIGHT_OF (node->left);
  avltree_height_t rh = HEIGHT_OF (node->right);
  return (node->height = (lh > rh ? lh : rh) + 1);
}

static inline avltree_node_t *
rotate_left (avltree_node_t *node)
{
  avltree_node_t *child = node->right;

  node->right = child->left;
  child->left = node;

  height_update (node);
  height_update (child);
  return child;
}

static inline avltree_node_t *
rotate_right (avltree_node_t *node)
{
  avltree_node_t *child = node->left;

  node->left = child->right;
  child->right = node;

  height_update (node);
  height_update (child);
  return child;
}

static inline avltree_node_t *
rotate (avltree_node_t *node)
{
  avltree_height_t bf = BALANCE_FACTOR_OF (node);

  if (bf > 1)
    {
      if (BALANCE_FACTOR_OF (node->left) >= 0)
        return rotate_right (node);

      node->left = rotate_left (node->left);
      return rotate_right (node);
    }

  if (bf < -1)
    {
      if (BALANCE_FACTOR_OF (node->right) <= 0)
        return rotate_left (node);

      node->right = rotate_right (node->right);
      return rotate_left (node);
    }

  return node;
}

avltree_node_t *
avltree_remove (avltree_t *tree, const avltree_node_t *target)
{
  avltree_height_t num = 0;
  avltree_node_t *parents[HEIGHT_MAX];
  avltree_height_t heights[HEIGHT_MAX];

  avltree_node_t *node = NULL;
  avltree_node_t **rmpos = &tree->root;
  avltree_comp_t *const f_comp = tree->comp_fn;

  for (avltree_node_t *curr = tree->root; curr;)
    {
      int comp = f_comp (target, curr);

      if (comp == 0)
        {
          node = curr;
          break;
        }

      parents[num] = curr;
      heights[num++] = curr->height;
      curr = *(rmpos = comp < 0 ? &curr->left : &curr->right);
    }

  if (!node)
    return NULL;

  if (!node->left && !node->right)
    {
      *rmpos = NULL;
      goto balance;
    }

  if (!node->left ^ !node->right)
    {
      *rmpos = node->left ?: node->right;
      goto balance;
    }

  avltree_height_t index = num++;
  avltree_node_t *node2 = node->right;
  avltree_node_t **rmpos2 = &node2->right;

  for (avltree_node_t *left; (left = node2->left);)
    {
      parents[num] = node2;
      heights[num++] = node2->height;
      rmpos2 = &node2->left;
      node2 = left;
    }

  avltree_node_t *next = node2->right;
  heights[index] = node->height;
  parents[index] = node2;

  *node2 = *node;
  *rmpos2 = next;
  *rmpos = node2;


balance:
  for (; num; num--)
    {
      avltree_node_t *curr = parents[num - 1];
      avltree_height_t height = heights[num - 1];

      avltree_height_t updated = height_update (curr);
      avltree_node_t *rotated = rotate (curr);

      if (rotated == curr)
        {
          if (updated == height)
            break;
          continue;
        }

      avltree_node_t **repos = &tree->root;
      if (num > 1)
        {
          avltree_node_t *father = parents[num - 2];
          repos = (curr == father->left) ? &father->left : &father->right;
        }
      *repos = rotated;
    }

  tree->size--;
  return node;
}

avltree_node_t *
avltree_find (const avltree_t *tree, const avltree_node_t *target)
{
  if (!tree->size)
    return NULL;

  avltree_comp_t *const f_comp = tree->comp_fn;
  for (avltree_node_t *curr = tree->root; curr;)
    {
      int comp = f_comp (target, curr);

      if (comp == 0)
        return curr;

      curr = comp < 0 ? curr->left : curr->right;
    }

  return NULL;
}

avltree_node_t *
avltree_insert (avltree_t *tree, avltree_node_t *node)
{
  avltree_height_t num = 0;
  avltree_node_t *parents[HEIGHT_MAX];
  avltree_height_t heights[HEIGHT_MAX];

  avltree_node_t **inpos = &tree->root;
  avltree_comp_t *const f_comp = tree->comp_fn;

  for (avltree_node_t *curr = tree->root; curr;)
    {
      int comp = f_comp (node, curr);

      if (comp == 0)
        return NULL;

      parents[num] = curr;
      heights[num++] = curr->height;
      curr = *(inpos = comp < 0 ? &curr->left : &curr->right);
    }

  *inpos = node;

  for (; num; num--)
    {
      avltree_node_t *curr = parents[num - 1];
      if (heights[num - 1] == height_update (curr))
        break;

      avltree_node_t *rotated = rotate (curr);
      if (rotated == curr)
        continue;

      avltree_node_t **repos = &tree->root;
      if (num > 1)
        {
          avltree_node_t *father = parents[num - 2];
          repos = (curr == father->left) ? &father->left : &father->right;
        }
      *repos = rotated;
    }

  tree->size++;
  return node;
}
