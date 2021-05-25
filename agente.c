#include "data.h"
int main(int argc, char **argv)
{
    //Inicializar variables
    char *pipe = malloc(MAX_SIZE);
    char *agent = malloc(MAX_SIZE / 2);
    int current_time, fd[2];
    //validar parametros
    if (argc == 9)
    {
        if (strcmp(argv[1], "-s") == 0 && strcmp(argv[3], "-a") == 0 && strcmp(argv[5], "-t") == 0 && strcmp(argv[7], "-p") == 0)
        {
            strcpy(pipe, argv[8]);
            strcpy(agent, argv[2]);
            //Send agent's name
            write_pipe(fd[1], (char *)agent, strlen(agent), pipe, O_WRONLY);
            //Read current time to check the requested into the file
            read_pipe(fd[0], (int *)&current_time, sizeof(int), pipe, O_RDONLY);
        }
    }
    else
    {
        printf("El proceso debe iniciarse de la siguiente manera ./agente –s nombre –a archivosolicitudes –t tesolicitud –p pipecrecibe\n");
        exit(1);
    }
    //**************Reading FILE************************
    printf("Current time:%d\n", current_time);
    Reserva re;
    FILE *file = fopen(argv[4], "r");
    char *pnt, *line = malloc(MAX_SIZE);
    if (file)
    {
        //leee del archivo
        while (fgets(line, MAX_SIZE, file) != NULL && line[0] != 44)
        {
            memset((reserva *)&re, 0, sizeof(reserva));
            re.status = -1;
            int k = 0;
            //quita los espacios de una linea
            line = drop_space(line);
            pnt = strtok(line, ",");
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
            //comprueba que la hora de la reserva no sea menor a la actual
            if (atoi(re.time) >= current_time)
            {
                char tmp[MAX_SIZE];
                strcpy(tmp, re.family_name);
                write_pipe(fd[1], &re, sizeof(re), agent, O_WRONLY);
                read_pipe(fd[0], &re, sizeof(re), agent, O_RDONLY);
                if (re.status == 0)
                    printf("Solicitud familia:%s \tEstado:aprobado\n", tmp);
                else if (re.status == 1)
                    printf("Solicitud familia:%s \tEstado:reprogramado\n", tmp);
                else
                    printf("Solicitud familia:%s \tEstado:negado\n", tmp);
            }
            else
                printf("Solicitud familia:%s,no enviada hora inferior a la actual\n", re.family_name);
            //Simula el tiempo de lectura de cada linea
            sleep(atoi(argv[6]));
        }
    }
    else
        printf("EL archivo de nombre %s no existe o no abre.\n", argv[4]);
    //Enviar una estrctura vacia
    memset(&re, 0, sizeof(Reserva));
    write_pipe(fd[1], &re, sizeof(re), agent, O_WRONLY);
    //eliminar los fifo creados
    char *command = malloc(20);
    strcpy(command, "rm ");
    strcat(command, agent);
    system(command);
}