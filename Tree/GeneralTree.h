#include "list.h"
typedef struct NodeGeneral
{
    void *data;
    list *dec;
} GeneralNode;
typedef struct GeneralTree
{
    GeneralNode *root;
} GeneralTree;
GeneralNode *init_GeneralNode(void *data)
{
    GeneralNode *newNode = (GeneralNode *)malloc(sizeof(GeneralNode));
    newNode->data = data;
    newNode->dec = NULL;
    return newNode;
}
/**
 * @brief Intialize tree and set the root 
 * @param tree GeneralTree is gonna be initialize
 * @param value  value to set the value of the root
 */
void init_GeneralTree(GeneralTree **tree, void *value)
{
    GeneralTree *tmp = (GeneralTree *)malloc(sizeof(GeneralTree));
    tmp->root = init_GeneralNode(value);
    *tree = tmp;
}
/**
 * @brief AddDesc in a speciific node
 * @param root root of the tree
 * @param parent Node that repesent the parent node where you want to add a new value
 * @param value Value to make a new node
 */
int insertNode(GeneralNode **current, void *parent, void *value)
{
    int retorno = NOT;
    GeneralNode *ct = *current;
    GeneralNode *newG = init_GeneralNode(value);
    if (ct != NULL && ct->data == parent)
    {
        push_back(&ct->dec, newG);
        retorno = OK;
    }
    else if (ct != NULL && ct->dec != NULL)
    {
        Node *node = ct->dec->head;
        while (node != NULL && retorno == NOT)
        {
            GeneralNode *tmp = (GeneralNode *)node->data;
            retorno = insertNode(&tmp, parent, value);
            node = node->next;
        }
    }
    return retorno;
}
int leafNode(GeneralNode *node)
{
    return OK ? node->dec == NULL : NOT;
}