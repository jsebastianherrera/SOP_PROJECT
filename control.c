#include "data.h"
int main(int argc, char **argv)
{
    char *pipe = malloc(MAX_SIZE);
    if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-f") == 0 && strcmp(argv[5], "-s") == 0 && strcmp(argv[7], "-t") == 0 && strcmp(argv[9], "-p") == 0)
    {
        strcpy(pipe, argv[10]);
    }
    else
    {
        printf("Invalid parameters\n");
        exit(1);
    }
    //************************************************************************************************
    data dt;
    mode_t fifo_mode = S_IRUSR | S_IWUSR;
    unlink(pipe);
    if (mkfifo(pipe, fifo_mode) == -1)
    {
        perror("mkfifo");
        exit(1);
    }
    int fd[2];
    read_pipe(fd[0], (data *)&dt, sizeof(dt), pipe);
    print(dt);
}