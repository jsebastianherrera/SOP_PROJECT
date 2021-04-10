#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "reserva.h"
#define MAX_RESERVARTION 20
typedef struct data
{
    reserva reservation[MAX_RESERVARTION];
    size_t size;
} data;
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
void read_pipe(int fd, void *buf, size_t size, char *pipe)
{
    int flag = 0, bytes;
    do
    {
        fd = open(pipe, O_RDONLY);
        if (fd == -1)
        {
            perror("pipe");
            printf(" Se volvera a intentar despues\n");
            sleep(5);
        }
        else
            flag = 1;
    } while (flag == 0);
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
    for (int i = 0; i < dt.size; i++)
        printf("Familia:%s - Hora:%d - #Personas:%d\n", dt.reservation[i].family_name, dt.reservation[i].time, dt.reservation[i].amount_people);
}
