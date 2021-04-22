#include "data.h"
void set_tree(GeneralTree **tree);
void printTree(GeneralNode *ct);
int main(int argc, char **argv)
{
    char *pipe = malloc(MAX_SIZE);
    beach bh;
    int fd[2];
    if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-f") == 0 && strcmp(argv[5], "-s") == 0 && strcmp(argv[7], "-t") == 0 && strcmp(argv[9], "-p") == 0)
    {
        data dt;
        strcpy(pipe, argv[10]);
        memset(&dt, 0, sizeof(dt));
        memset(&bh, 0, sizeof(beach));
        char *agent_name = malloc(MAX_SIZE / 2);
        bh.current_time = atoi(argv[2]);
        bh.max_people = atoi(argv[8]);
        mode_t fifo_mode = S_IRUSR | S_IWUSR;
        unlink(pipe);
        if (mkfifo(pipe, fifo_mode) == -1)
        {
            perror("mkfifo");
            exit(1);
        }
        read_pipe(fd[0], agent_name, MAX_SIZE / 2, pipe, O_RDONLY);
        write_pipe(fd[1], (int *)&bh.current_time, sizeof(int), pipe, O_WRONLY);
        printf("\nAgente conectado: %s\n", agent_name);
    }
    else
    {
        printf("Invalid parameters\n");
        exit(1);
    }
    //**************************************************
    GeneralTree *tree;
    init_GeneralTree(&tree, "Horas");
    set_tree(&tree);
    GeneralNode *node = tree->root;
    int cont = 0;
    struct reserva re[20];
    do
    {
        read_pipe(fd[0], (struct Reserva *)&re[cont], sizeof(Reserva), pipe, O_RDONLY);
        if (re[cont].amount_people == 0)
            break;
        else
        {
            answer_request(tree, &re[cont], &bh);
            //insertNode(&tree->root, (char *)&re[cont].time, strcat((char *)&re[cont].family_name, int_to_char(re[cont].amount_people, 0)));
        }
        write_pipe(fd[1], (struct Reserva *)&re[cont], sizeof(Reserva), pipe, O_WRONLY);
        cont++;
    } while (1);
    //****************************************************************
    printf("\n");
    printTree(tree->root);
    printf("Gente actual::%d\n", bh.amount_people);
    //TODO:Data stucture
}

void set_tree(GeneralTree **tree)
{
    for (int i = 7; i <= 20; ++i)
        insertNode(&(*tree)->root, "Horas", int_to_char(i, 1));
}
void printTree(GeneralNode *ct)
{
    if (ct != NULL && ct->dec != NULL)
    {
        Node *node = ct->dec->head;
        while (node != NULL)
        {
            GeneralNode *tmp = (GeneralNode *)node->data;
            printTree(tmp);
            node = node->next;
        }
    }
    if (ct != NULL)
        printf("%s\n", (char *)(ct->data));
}
