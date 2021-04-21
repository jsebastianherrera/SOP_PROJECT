#include "GeneralTree.h"
#define COUNT 10
void print(GeneralNode *, int);
int main(void)
{
  GeneralTree *tree;
  int r = 0, a = 1, b = 2, c = 3, d = 4, e = 5,q=232332;
  init_GeneralTree(&tree, (int *)&r);
  insertNode(&tree->root, (int *)&r, (int *)&a);
  insertNode(&tree->root, (int *)&r, (int *)&b);
  insertNode(&tree->root, (int *)&r, (int *)&c);
  insertNode(&tree->root, (int *)&c, (int *)&c);
  insertNode(&tree->root, (int *)&r, (int *)&d);
  insertNode(&tree->root, (int *)&d, (int *)&q);
  //deleteDec(&tree->root, (int *)&a);
  print(tree->root, 0);
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
