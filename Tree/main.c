#include "GeneralTree.h"
#define COUNT 10
void print(GeneralNode *, int);
int main(void)
{
  GeneralTree *tree;
  int r = 0, x = 1, y = 2, v = 3;
  init_GeneralTree(&tree, (int *)&r);
  insertNode(&tree->root, (int *)&r, (int *)&x);
  int space = 0;
  print(tree->root, space);
  getchar();
}
void print(GeneralNode *ct, int space)
{
  if (ct != NULL && ct->dec != NULL)
  {
    Node *node = ct->dec->head;
    while (node != NULL)
    {
      space += COUNT;
      GeneralNode *tmp = (GeneralNode *)*((size_t *)node->data);
      print(tmp, space);
      node = node->next;
    }
  }
  else if (ct != NULL)
  {
    printf("\n");
    for (int i = COUNT; i < space; i++)
      printf(" ");
    printf("%d\n", *(int *)ct->data);
  }
}
