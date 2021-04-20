#include "GeneralTree.h"
#define COUNT 10
void print(GeneralNode *, int);
int main(void)
{
  GeneralTree *tree;
  int r = 0, a = 1, b = 2, c = 3, d = 4, e = 5;
  init_GeneralTree(&tree, (int *)&r);
  insertNode(&tree->root, (int *)&r, (int *)&a);
  insertNode(&tree->root, (int *)&r, (int *)&b);
  insertNode(&tree->root, (int *)&r, (int *)&c);
  insertNode(&tree->root, (int *)&r, (int *)&d);
  insertNode(&tree->root, (int *)&r, (int *)&e);
  deleteDec(&tree->root, (int *)&r);
  int space = 0;
  print(tree->root, space);
}
void print(GeneralNode *ct, int space)
{
  if (ct != NULL && ct->dec != NULL)
  {
    Node *node = ct->dec->head;
    while (node != NULL)
    {
      space += COUNT;
      GeneralNode *tmp = (GeneralNode *)node->data;
      print(tmp, space);
      node = node->next;
    }
  }
  if (ct != NULL)
    printf("%d\n", *(int *)ct->data);
}
