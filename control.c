#include "data.h"
GeneralTree *tree;
beach bh;
//Estructura donde se almacena las reservas
typedef struct proof
{
    struct reserva *re;
} data;
//Inicializar el arbol
void set_tree(GeneralTree **tree, char *, char *);
//Imprimir arbol
void printTree(GeneralNode *ct);
//Routina para avisar cuando una hora ya paso y imprimir la gente que sale y entra
void *routineTime(void *val)
{
    printf("Current time: %d\n", bh.current_time);
    while (bh.current_time <= bh.close_time)
    {
        sleep(*(int *)val);
        printf("\n**************************************\n");
        update_tree(&tree, getAmountPeopleByHour(&tree, bh.current_time), bh.current_time);
        printTree(tree->root);
        printf("\n\n");
        bh.current_time++;
        printf("Current time: %d\n", bh.current_time);
    }
    report(tree, bh);
    kill(getpid(), 9);
}
//Lee del pipe
void *routineRead(void *val)
{
    int fg = 0, fd;
    void *buf = malloc(MAX_SIZE * 2);
    do
    {
        fd = open(val, O_RDONLY);
        if (fd == -1)
        {
            perror("pipe");
            printf(" Se volvera a intentar despues\n");
            sleep(5);
        }
        else
            fg = 1;
    } while (fg == 0);
    read(fd, buf, MAX_SIZE * 2);
    close(fd);
    return buf;
}
//Escribe en el pipe para enviar la hora
void *routineWrite(void *val)
{
    int fg = 0, fd;
    int *time = malloc(sizeof(int));
    *time = bh.current_time;
    do
    {
        fd = open(val, O_WRONLY);
        if (fd == -1)
        {
            perror("pipe");
            printf(" Se volvera a intentar despues\n");
            sleep(5);
        }
        else
            fg = 1;
    } while (fg == 0);
    write(fd, time, MAX_SIZE);
    close(fd);
}
int main(int argc, char **argv)
{
    //inicializar variables
    int fd;
    data data[20];
    char *pipe = malloc(MAX_SIZE);
    char *agent_name = malloc(MAX_SIZE / 2);
    struct reserva *re = malloc(20);
    //validar que los parametros esten completos
    if (argc == 11)
    {
        if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-f") == 0 && strcmp(argv[5], "-s") == 0 && strcmp(argv[7], "-t") == 0 && strcmp(argv[9], "-p") == 0)
        {
            //Inicializar variables
            strcpy(pipe, argv[10]);
            memset(&bh, 0, sizeof(beach));
            bh.inti_time = atoi(argv[2]);
            bh.current_time = atoi(argv[2]);
            bh.max_people = atoi(argv[8]);
            bh.close_time = atoi(argv[4]);
            bh.amount_people = 0;
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
    set_tree(&tree, int_to_char(bh.inti_time, 1), int_to_char(bh.close_time, 1));
    pthread_t p_read, p_write, p_time;
    //Inicializar el file descriptor
    clean_fifo(pipe);
    int seconds = atoi(argv[6]);
    pthread_create(&p_time, NULL, routineTime, &seconds);
    do
    {
        //Lee y escribe en el pipe
        int cont = 0;
        //p_read get the agent name from the pipe
        read_pipe(fd, (char *)agent_name, MAX_SIZE, pipe, O_RDONLY);
        printf("Agente:%s\t", agent_name);
        clean_fifo(agent_name);
        //send curret hour
        pthread_create(&p_write, NULL, routineWrite, pipe);
        printf("pipe: %s\n", agent_name);
        //Read all the requested by an agent
        do
        {
            pthread_create(&p_read, NULL, routineRead, agent_name);
            pthread_join(p_read, (void **)&data[cont].re);
            if (data[cont].re->amount_people != 0)
                answer_request(&tree, data[cont].re, &bh);
            else
                break;
            write_pipe(fd, (struct Reserva *)data[cont].re, sizeof(Reserva), agent_name, O_WRONLY);
            cont++;
        } while (1);

    } while (1);
}
void set_tree(GeneralTree **tree, char *current_time, char *final_hour)
{
    //Inicializa la cabeza de la siguiente forma (tiempo,cantidad actual)
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
