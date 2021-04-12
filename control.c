#include "data.h"
int main(int argc, char **argv)
{
    char *pipe = malloc(MAX_SIZE);
    if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-f") == 0 && strcmp(argv[5], "-s") == 0 && strcmp(argv[7], "-t") == 0 && strcmp(argv[9], "-p") == 0)
    {
        data dt;
        beach bh;
        strcpy(pipe, argv[10]);
        memset(&dt, 0, sizeof(dt));
        memset(&bh, 0, sizeof(beach));
        char *agent_name = malloc(MAX_SIZE / 2);
        bh.current_time = atoi(argv[2]);
        bh.max_people = atoi(argv[8]);
        mode_t fifo_mode = S_IRUSR | S_IWUSR;
        unlink(pipe);
        if (mkfifo(pipe, fifo_mode) == -1)
        {
            perror("mkfifo");
            exit(1);
        }
        int fd[2];
        read_pipe(fd[0], agent_name, MAX_SIZE / 2, pipe, O_RDONLY);
        printf("Agente:%s\n", agent_name);
        write_pipe(fd[1], (int *)&bh.current_time, sizeof(int), pipe);
        read_pipe(fd[0], (struct data *)&dt, sizeof(struct data), pipe, O_RDONLY);
        print(dt);
        simulate_time(2, 2, 1); //*add to current time the simulated hours argv[2]
        bh.current_time += 2;
        PRINT_VAR(bh.current_time);
    }
    else
    {
        printf("Invalid parameters\n");
        exit(1);
    }
}
