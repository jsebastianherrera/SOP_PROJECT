#include "data.h"
void read_file(data *, char *);
int main(int argc, char **argv)
{
    char *pipe = malloc(MAX_SIZE);
    if (strcmp(argv[1], "-s") == 0 && strcmp(argv[3], "-a") == 0 && strcmp(argv[5], "-t") == 0 && strcmp(argv[7], "-p") == 0)
    {
        strcpy(pipe, argv[8]);
    }
    else
    {
        printf("Invalid parameters\n");
        exit(1);
    }
    int fd[2];
    data dt;
    memset((data *)&dt, 0, sizeof(dt));
    read_file(&dt, argv[4]);
    write_pipe(fd[1], (data *)&dt, sizeof(dt), pipe);
}
void read_file(data *dt, char *file)
{
    int i = 0;
    FILE *fd = fopen(file, "r"); 
    char *pnt, *line =malloc(MAX_SIZE);
    while (fgets(line, MAX_SIZE, fd) != NULL && line[0] != 44)
    {

        int k = 0;
        pnt = strtok(line, ",");
        line = drop_space(line);
        while (pnt != NULL && k <= 2)
        {
            if (k == 0)
                strcpy(dt->reservation[i].family_name, pnt);
            else if (k == 1)
                dt->reservation[i].time = atoi(pnt);
            else
                dt->reservation[i].amount_people = atoi(pnt);
            k++;
            pnt = strtok(NULL, ",");
        }
        i++;
    }
    dt->size = i;
}