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
    read_file(&dt, argv[4]);
    print(dt);
    //   write_pipe(fd[1], (data *)&dt, sizeof(dt), pipe);
}
void read_file(data *dt, char *file)
{

    int i = 0, k;
    FILE *fd = fopen(file, "r");
    char *pnt, *line = malloc(MAX_SIZE);
    while (fgets(line, sizeof(line), fd) != NULL && line[0] != 44)
    {
        k = 0;
        line = drop_space(line);
        pnt = strtok(line, ",");
        strcpy(dt->reservation[i].family_name, pnt);
        while (pnt != NULL)
        {
            pnt = strtok(NULL, ",");
            if (k == 0)
                dt->reservation[i].time = atoi(pnt);
            else
                dt->reservation[i].amount_people = atoi(pnt);
            k++;
        }
        i++;
    }
    dt->size = i;
}