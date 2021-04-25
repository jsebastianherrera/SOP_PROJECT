//#include "GeneralTree.h"
#define COUNT 10
#include <stdio.h>
#include <string.h>
//void print(GeneralNode *, int);
int main(void)
{
  /*GeneralTree *tree;
  int r = 0, a = 1, b = 2, c = 3, d = 4, e = 5, q = 232332;
  init_GeneralTree(&tree, (int *)&r);
  insertNode(&tree->root, (int *)&r, (int *)&a);
  insertNode(&tree->root, (int *)&r, (int *)&b);
  insertNode(&tree->root, (int *)&r, (int *)&c);
  //deleteDec(&tree->root, (int *)&a);
  print(tree->root, 0);
  GeneralNode *aux = search(tree->root, (int *)&d);
  if (aux != NULL)
  {
    printf("::nice::%d\n",size_list(tree->root->dec));
  }*/
  char val[20] = "(7,0)";
  char *pnt = strtok(val, "(,");
  while (pnt != NULL)
  {
    printf("%s\n", pnt);
    pnt = strtok(NULL, ")");
  }
}
/*void print(GeneralNode *ct, int space)
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
*/