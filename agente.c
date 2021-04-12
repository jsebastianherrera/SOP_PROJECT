#include "data.h"
void read_file(data *, char *);
int main(int argc, char **argv)
{
    char *pipe = malloc(MAX_SIZE);
    if (strcmp(argv[1], "-s") == 0 && strcmp(argv[3], "-a") == 0 && strcmp(argv[5], "-t") == 0 && strcmp(argv[7], "-p") == 0)
    {
        strcpy(pipe, argv[8]);
        int fd[2];
        data dt;
        memset((data *)&dt, 0, sizeof(dt));
        strcpy(dt.agent_name, argv[2]);
        //Send agent's name
        write_pipe(fd[1], (char *)&dt.agent_name, strlen(dt.agent_name), pipe);
        int current_time;
        //Read current time to check the requested into the file
        read_pipe(fd[0], (int *)&current_time, sizeof(int), pipe, O_RDONLY);
        printf("Time:%d\n", current_time);
        //Read file to get the information from the file and fill it into our data structure
        read_file(&dt, argv[4]);
        //Sent the information to the control process
        write_pipe(fd[1], (struct data *)&dt, sizeof(struct data), pipe);
    }
    else
    {
        printf("Invalid parameters\n");
        exit(1);
    }
}
void read_file(data *dt, char *file)
{
    int i = 0;
    Reserva ra;
    FILE *fd = fopen(file, "r");
    char *pnt, *line = malloc(MAX_SIZE);
    while (fgets(line, MAX_SIZE, fd) != NULL && line[0] != 44)
    {
        memset((reserva *)&ra, 0, sizeof(reserva));
        int k = 0;
        pnt = strtok(line, ",");
        line = drop_space(line);
        while (pnt != NULL && k <= 2)
        {
            if (k == 0)
                strcpy(ra.family_name, pnt);
            else if (k == 1)
                ra.time = atoi(pnt);
            else
                ra.amount_people = atoi(pnt);
            k++;
            pnt = strtok(NULL, ",");
            dt->reservation[i] = ra;
        }
        i++;
    }
    dt->size = i;
}