#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "playa.h"
#include "reserva.h"
#include "Tree/GeneralTree.h"
#define MAX_RESERVARTION 20
#define PRINT_VAR(X) printf("Values:%d\n", X);
typedef struct data
{
    char agent_name[MAX_SIZE];
    reserva reservation[MAX_RESERVARTION];
    size_t size;
} data;
/**
 * @brief  function to simulated time
 * @param argc number of parameters 
 * @param ... time to simulate(hour), seconds per hour
 */
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
int check_time(int compareTo, int current_time)
{
    return -1 ? compareTo < current_time : 0;
}
void write_pipe(int fd, void *buf, size_t size, char *pipe, int flag)
{
    int boolean = 0, bytes;
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
            boolean = 1;
    } while (boolean == 0);
    bytes = write(fd, buf, size);
   // printf("Sent it:%d\n", bytes);
    close(fd);
}
void read_pipe(int fd, void *buf, size_t size, char *pipe, int flag)
{
    int fg = 0, bytes;
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
    bytes = read(fd, buf, size);
   // printf("Received it:%d\n", bytes);
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
void print(data dt)
{
    printf("Solicitudes agente:%s\n", dt.agent_name);
    for (int i = 0; i < dt.size; i++)
        printf("Familia:%s - Hora:%s - #Personas:%d\n", dt.reservation[i].family_name, dt.reservation[i].time, dt.reservation[i].amount_people);
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
void update_beach(GeneralTree *tree, beach *bh)
{
    GeneralNode *found = search(tree->root, int_to_char(bh->current_time, 1));
    int i = 0, sum = 0;
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
            sum += atoi(token);
            node = node->next;
        }
    }
    bh->amount_people = sum;
}
int getAmountPeopleByHour(GeneralTree **tree, int time)
{
    int rt = 0;
  //  printf("TIME::%d\n", getCurrentTime((*tree)->root));
    GeneralNode *found = search((*tree)->root,int_to_char(time,1));
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
int answer_request(GeneralTree **tree, reserva *re, beach *bh)
{
    int rt = 1;
    GeneralNode *found = search((*tree)->root, re->time);
    if (found != NULL)
    {
        update_beach(*tree, bh);
        if (getAmountPeople((*tree)->root) + re->amount_people <= bh->max_people)
        {
            re->status = 0;
            char *a;
            a = strcat(re->family_name, int_to_char(re->amount_people, 0));
            int time = atoi(re->time);
            for (int i = 0; i <= 1; ++i)
            {
                time += i;
                insertNode(&(*tree)->root, int_to_char(time, 1), a);
            }
            update_tree(tree, getAmountPeopleByHour(tree,atoi(re->time)), bh->current_time);
        }
    }
    else
        return -1;
    return rt;
}