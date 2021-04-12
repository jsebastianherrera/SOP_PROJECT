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
int answer_request(reserva re, beach bh, int current_time, int max_people)
{
    if (bh.amount_people <= max_people)
        return -1;
}
int check_time(int compareTo, int current_time)
{
    return -1 ? compareTo < current_time : 0;
}
void write_pipe(int fd, void *buf, size_t size, char *pipe)
{
    int flag = 0, bytes;
    do
    {
        fd = open(pipe, O_WRONLY);
        if (fd == -1)
        {
            perror("pipe");
            printf(" Se volvera a intentar despues\n");
            sleep(5);
        }
        else
            flag = 1;
    } while (flag == 0);
    bytes = write(fd, buf, size);
    printf("Sent it:%d\n", bytes);
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
    printf("Received it:%d\n", bytes);
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
        printf("Familia:%s - Hora:%d - #Personas:%d\n", dt.reservation[i].family_name, dt.reservation[i].time, dt.reservation[i].amount_people);
}
