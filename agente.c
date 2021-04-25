#include "data.h"
int main(int argc, char **argv)
{
    char *pipe = malloc(MAX_SIZE);
    char *agent = malloc(MAX_SIZE / 2);
    int current_time, fd[2];
    if (strcmp(argv[1], "-s") == 0 && strcmp(argv[3], "-a") == 0 && strcmp(argv[5], "-t") == 0 && strcmp(argv[7], "-p") == 0)
    {
        strcpy(pipe, argv[8]);
        strcpy(agent, argv[2]);
        //Send agent's name
        write_pipe(fd[1], (char *)agent, strlen(agent), pipe, O_WRONLY);
        //Read current time to check the requested into the file
        read_pipe(fd[0], (int *)&current_time, sizeof(int), pipe, O_RDONLY);
    }
    else
    {
        printf("Invalid parameters\n");
        exit(1);
    }
    //**************Reading FILE************************
    Reserva re;
    FILE *file = fopen(argv[4], "r");
    char *pnt, *line = malloc(MAX_SIZE);
    while (fgets(line, MAX_SIZE, file) != NULL && line[0] != 44)
    {
        memset((reserva *)&re, 0, sizeof(reserva));
        int k = 0;
        pnt = strtok(line, ",");
        line = drop_space(line);
        while (pnt != NULL && k <= 2)
        {
            if (k == 0)
                strcpy(re.family_name, pnt);
            else if (k == 1)
                strcpy(re.time, pnt);
            else
                re.amount_people = atoi(pnt);
            k++;
            pnt = strtok(NULL, ",");
        }
        if (atoi(re.time) >= current_time)
        {

            write_pipe(fd[1], &re, sizeof(re), pipe, O_WRONLY);
            simulate_time(2, 1, atoi(argv[6]));
            read_pipe(fd[0], &re, sizeof(re), pipe, O_RDONLY);
            current_time = re.current_time;
        }
    }
    memset(&re, 0, sizeof(Reserva));
    write_pipe(fd[1], &re, sizeof(re), pipe, O_WRONLY);
}