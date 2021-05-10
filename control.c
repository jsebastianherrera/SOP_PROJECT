#include "data.h"
void set_tree(GeneralTree **tree, char *, char *);
void printTree(GeneralNode *ct);
int main(int argc, char **argv)
{
    char *pipe = malloc(MAX_SIZE);
    beach bh;
    int fd[2];
    do
    {
        if (argc == 11)
        {
            if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-f") == 0 && strcmp(argv[5], "-s") == 0 && strcmp(argv[7], "-t") == 0 && strcmp(argv[9], "-p") == 0)
            {
                strcpy(pipe, argv[10]);
                memset(&bh, 0, sizeof(beach));
                char *agent_name = malloc(MAX_SIZE / 2);
                bh.current_time = atoi(argv[2]);
                bh.max_people = atoi(argv[8]);
                bh.close_time = atoi(argv[4]);
                bh.amount_people = 0;
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
        }
        else
        {
            printf("El proceso debe iniciarse de la siguiente forma:\ncontrolador –i horaInicio –f horafinal –s segundoshora –t totalpersonas –p pipecrecibe\n");
            exit(0);
        }
        //**************************************************
        GeneralTree *tree;
        set_tree(&tree, int_to_char(bh.current_time, 1), int_to_char(bh.close_time, 1));
        GeneralNode *node = tree->root;
        int cont = 0;
        int init_time = bh.current_time;
        struct reserva re[20];
        do
        {
            read_pipe(fd[0], (struct Reserva *)&re[cont], sizeof(Reserva), pipe, O_RDONLY);
            if (re[cont].amount_people == 0)
                break;
            else
                answer_request(&tree, &re[cont], &bh);
            write_pipe(fd[1], (struct Reserva *)&re[cont], sizeof(Reserva), pipe, O_WRONLY);
            cont++;
        } while (1);
        report(tree);
    } while (1);
}
void set_tree(GeneralTree **tree, char *current_time, char *final_hour)
{
    char *time = malloc(sizeof(char *));
    strcat(time, "(");
    strcat(time, current_time);
    strcat(time, ",");
    strcat(time, "0)");
    char *tmp = malloc(sizeof(char *));
    strcpy(tmp, time);
    init_GeneralTree(tree, time);
    for (int i = atoi(current_time); i <= atoi(final_hour); ++i)
        insertNode(&(*tree)->root, time, int_to_char(i, 1));
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
