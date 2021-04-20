#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define NOT 0
typedef struct node
{
    void *data;
    struct node *next;
} node, Node;
typedef struct list
{
    struct node *head;

} list, List;
//*Functions
node *newNode(void *value)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}
void init_list(list **lista, void *value)
{
    list *aux = *lista;
    aux = (list *)malloc(sizeof(list));
    aux->head = newNode(value);
    *lista = aux;
}
void push_back(list **lista, void *value)
{
  
    if (*lista != NULL)
    {
          node *aux = (*lista)->head;
        node *nw = newNode(value);
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = nw;
    }
    else
        init_list(lista, value);
}
int delete_node(list **lista, void *value)
{
    if ((*lista)->head != NULL)
    {
        //Check the head first if you want to remove a node, if the value is equal, we need to point to the head of the list with the following pointer
        node *head = (*lista)->head;
        if (head != NULL && head->data == value)
        {
            (*lista)->head = head->next;
            free(head);
            return OK;
        }
        /**
         * Stop if the value was found or if the head is null
         *1.If the value was found it's mean that the "prev" pointer it's gonna be the pointer before the node that it's gonna be remove
         and in the next pointer("pre->next") we'll store the "next" pointer from the node that we'll delete. 
         * 
         */
        node *prev = NULL;
        while (head != NULL && head->data != value)
        {
            prev = head;
            head = head->next;
        }
        //If head is null it's mean that after visiting each node the value wasn't found.
        if (head == NULL)
            return NOT;

        prev->next = head->next;
        return OK;
    }
    return NOT;
}