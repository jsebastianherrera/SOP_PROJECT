int solicitudes_negadas = 0, solicitudes_aprobadas = 0, solicitudes_reprogramadas = 0;
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#include "playa.h"
#include <pthread.h>
#include "reserva.h"
#include <semaphore.h>
#include "Tree/GeneralTree.h"
#define MAX_RESERVARTION 20
#define LONG 4
/**
 * @brief  function to simulated time
 * @param argc number of parameters 
 * @param ... time to simulate(hour), seconds per hour
 */
typedef struct
{
    char string[80];
} String;
void clean_fifo(char *pipe)
{
    mode_t fifo_mode = S_IRUSR | S_IWUSR;
    unlink(pipe);
    if (mkfifo(pipe, fifo_mode) == -1)
    {
        perror("mkfifo");
        exit(1);
    }
}
char *randomChar()
{
    char *rt = malloc(LONG);
    srand(time(NULL));
    for (int i = 0; i < 4; i++)
        rt[i] = 65 + rand() % (90 - 65);
    srand(time(NULL));
    return (char *)rt;
}
void simulate_time(const int argc, ...)
{
    //! Testing function
    int *values = malloc(argc);
    va_list args;
    va_start(args, argc);
    for (int i = 0; i < argc; ++i)
        values[i] = va_arg(args, int);
    va_end(args);
    //TODO: simulated time
    for (int i = 0; i < values[0]; ++i)
        sleep(values[1]);
}
void write_pipe(int fd, void *buf, size_t size, char *pipe, int flag)
{
    int boolean = 0;
    fd = open(pipe, flag);
    if (fd == -1)
    {
        perror("pipe");
        printf(" Se volvera a intentar despues\n");
        sleep(5);
    }
    else
        boolean = 1;
    write(fd, buf, size);
    close(fd);
}
void read_pipe(int fd, void *buf, size_t size, char *pipe, int flag)
{
    int fg = 0;
    do
    {
        fd = open(pipe, flag);
        if (fd == -1)
        {
            perror("pipe");
            printf(" Se volvera a intentar despues\n");
            sleep(5);
        }
        else
            fg = 1;
    } while (fg == 0);
    read(fd, buf, size);
    close(fd);
}
char *drop_space(char *line)
{
    char *rt = malloc(strlen(line));
    int k = 0;
    for (int i = 0; i < strlen(line); ++i)
    {
        if (line[i] != 32)
        {
            rt[k] = line[i];
            k++;
        }
    }
    return rt;
}
char *int_to_char(int num, int param)
{
    char *ret = malloc(4);
    int i = 0;
    if (param == 0)
    {
        ret[i] = ',';
        i++;
    }
    if (num > 9)
    {
        ret[i] = num / 10 + 48;
        ret[i + 1] = num % 10 + 48;
    }
    else
        ret[i] = num + 48;
    return ret;
}
int getCurrentTime(GeneralNode *node)
{
    char *cpy = malloc(sizeof(char *));
    strcpy(cpy, node->data);
    char *token = strtok(cpy, "(,");
    return atoi(token);
}
int getAmountPeople(GeneralNode *node)
{
    char *cpy = malloc(sizeof(char *));
    strcpy(cpy, node->data);
    char *token = strtok(cpy, "(,");
    token = strtok(NULL, ")");
    return atoi(token);
}
//Sum time
void setHeadTime(GeneralNode **head, int time, int amount_people)
{
    char *_time = malloc(sizeof(char *));
    strcat(_time, "(");
    strcat(_time, int_to_char(time, 1));
    strcat(_time, ",");
    strcat(_time, int_to_char(amount_people, 1));
    strcat(_time, ")");
    strcpy((*head)->data, _time);
}
void update_tree(GeneralTree **tree, int amount_people, int current_time)
{
    if (*tree != NULL)
        setHeadTime(&(*tree)->root, current_time, amount_people);
}
int getAmountPeopleByHour(GeneralTree **tree, int time)
{
    int rt = 0;
    //  printf("TIME::%d\n", getCurrentTime((*tree)->root));
    GeneralNode *found = search((*tree)->root, int_to_char(time, 1));
    if (found != NULL && size_list(found->dec) > 0)
    {
        Node *node = found->dec->head;
        for (int i = 0; i < size_list(found->dec); i++)
        {
            GeneralNode *tmp = (GeneralNode *)node->data;
            char *cpy = malloc(MAX_SIZE);
            strcpy(cpy, tmp->data);
            char *token = strtok(cpy, ",");
            token = strtok(NULL, ",");
            rt += atoi(token);
            node = node->next;
        }
    }
    return rt;
}
int sufficent_space(GeneralTree *tree, reserva re, beach bh)
{
    int rt = -1;
    for (int i = atoi(re.time); i <= bh.close_time - 1; i++)
    {
        if (getAmountPeopleByHour(&tree, i) + re.amount_people <= bh.max_people)
        {
            if (getAmountPeopleByHour(&tree, i + 1) + re.amount_people <= bh.max_people)
                return i;
        }
    }
    return rt;
}
int answer_request(GeneralTree **tree, reserva *re, beach *bh)
{
    int rt = 0;
    GeneralNode *found = search((*tree)->root, re->time);
    if (found != NULL)
    {

        if (atoi(re->time) > bh->close_time)
        {
            re->status = 2;
            solicitudes_negadas++;
        }
        //Reserve ok
        else if (getAmountPeopleByHour(tree, atoi(re->time)) + re->amount_people <= bh->max_people && getAmountPeopleByHour(tree, atoi(re->time) + 1) + re->amount_people <= bh->max_people)
        {
            solicitudes_aprobadas++;
            re->status = 0;
            char *a;
            a = strcat(re->family_name, int_to_char(re->amount_people, 0));
            int time = atoi(re->time);
            for (int i = 0; i <= 1; ++i)
            {
                time += i;
                insertNode(&(*tree)->root, int_to_char(time, 1), a);
            }
        }
        //Max amount filled
        else
        {
            //printf("**Entrela**");
            if (sufficent_space(*tree, *re, *bh) != -1)
            {
                re->status = 1;
                char *a;
                a = strcat(re->family_name, int_to_char(re->amount_people, 0));
                int time = sufficent_space(*tree, *re, *bh);
                strcpy(re->time, int_to_char(time, 1));
                //**************************************
                for (int i = 0; i <= 1; ++i)
                {
                    time += i;
                    insertNode(&(*tree)->root, int_to_char(time, 1), a);
                }
                solicitudes_reprogramadas++;
            }
            else
            {
                printf("**Entrela**");
                re->status = 2;
                solicitudes_negadas++;
            }
        }
    }
    else
    {
        solicitudes_negadas++;
        return -1;
    }
    update_tree(tree, getAmountPeopleByHour(tree, atoi(re->time)), bh->current_time);
    return rt;
}
int *sort_bubble(GeneralTree *tree, int *tam, beach bh)
{
    int *arr = malloc(50), cont = 0;
    for (int i = bh.inti_time; i <= bh.close_time; i++)
    {
        if (getAmountPeopleByHour(&tree, i) != 0)
        {
            *(arr + cont) = getAmountPeopleByHour(&tree, i);
            cont++;
        }
    }
    //Sorted
    int aux;
    for (int i = 0; i < cont; i++)
    {
        for (int j = i + 1; j < cont; j++)
        {
            if (*(arr + i) < *(arr + j))
            {
                aux = *(arr + i);
                *(arr + i) = *(arr + j);
                *(arr + j) = aux;
            }
        }
    }
    *tam = cont;
    return arr;
}
char *horaPico(GeneralTree *tree, beach bh)
{
    int tam;
    char *rt = malloc(50);
    int *tmp = sort_bubble(tree, &tam, bh);
    if (tam > 0)
    {
        for (int i = 7; i <= 19; i++)
        {

            if (getAmountPeopleByHour(&tree, i) == *(tmp + 0))
            {
                strcat(rt, int_to_char(i, 1));
                strcat(rt, "  ");
            }
        }
    }
    return rt;
}
char *menorAfluencia(GeneralTree *tree, beach bh)
{
    int tam, k = 0;
    char *rt = malloc(50);
    int *tmp = sort_bubble(tree, &tam, bh);
    if (tam > 0)
    {
        for (int i = 7; i <= 19 && k == 0; i++)
        {
            if (getAmountPeopleByHour(&tree, i) == *(tmp + tam - 1))
            {
                strcat(rt, int_to_char(i, 1));
                strcat(rt, "  ");
            }
        }
    }
    return rt;
}
void report(GeneralTree *tree, beach bh)
{
    //sort_bubble(tree);
    printf("\t\t\t REPORTE\n");
    String mat[6][2];
    strcpy(mat[0][0].string, "Indicadores");
    strcpy(mat[0][1].string, "                                     Valor");
    strcpy(mat[1][0].string, "Horas pico");
    strcpy(mat[2][0].string, "Horas con menor afluencia");
    strcpy(mat[3][0].string, "Numero de solictudes negadas");
    strcpy(mat[4][0].string, "Numero de solicitudes aceptadas en su hora");
    strcpy(mat[5][0].string, "Numero de solictudes reprogramadas");
    strcpy(mat[1][1].string, "                                       ");
    strcat(mat[1][1].string, horaPico(tree, bh));
    strcpy(mat[2][1].string, "                       ");
    strcat(mat[2][1].string, menorAfluencia(tree, bh));
    strcpy(mat[3][1].string, "                       ");
    strcat(mat[3][1].string, int_to_char(solicitudes_negadas, 1));
    strcpy(mat[4][1].string, "       ");
    strcat(mat[4][1].string, int_to_char(solicitudes_aprobadas, 1));
    strcpy(mat[5][1].string, "               ");
    strcat(mat[5][1].string, int_to_char(solicitudes_reprogramadas, 1));

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 2; j++)
            printf("%s\t", mat[i][j].string);
        printf("\n");
    }
}