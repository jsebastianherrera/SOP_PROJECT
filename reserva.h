#define MAX_SIZE 1024
typedef struct reserva
{
    char family_name[MAX_SIZE];
    int amount_people;
    int current_time;
    //-1 undefined , 0 reserve ok,1 propose reserve, 2 not approved late,3 reject reserve
    int status;
    char time[2];
} Reserva, reserva;
