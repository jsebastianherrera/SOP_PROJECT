#include "GeneralTree.h"
int main(void)
{
  /*
  GeneralTree *tree;
  int r = 0, x = 1, y = 2, v = 1;
  init_GeneralTree(&tree, (int *)&r);
  insertNode(&tree->root, (int *)&r, (int *)&x);
  insertNode(&tree->root, (int *)&x, (int *)&y);
  //deleteDec(&tree->root, (int *)&x);
  //deleteDec(&tree->root, (int *)&r);*/

  List *list;
  GeneralNode *proof;
  int x = 5;
  proof = init_GeneralNode((int *)&x);
  init_list(&list, (GeneralNode *)&proof);
  //push_back(&list, (GeneralNode *)&proof);
  Node *aux = list->head;
  GeneralNode tmp;
  while (aux != NULL)
  {
    tmp = *(GeneralNode*)&aux->data;
    printf("::%d\n", *(int *)tmp.data);
    aux = aux->next;
  }
  getchar();
}
