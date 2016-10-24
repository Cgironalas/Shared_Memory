#include <stdio.h>
#include <time.h>

int main (){

    time_t rawtime;
    struct tm * timeinfo;

    int hora[6];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    hora[0] = timeinfo->tm_mday;
    hora[1] = timeinfo->tm_mon + 1;
    hora[2] = timeinfo->tm_year + 1900;
    hora[3] = timeinfo->tm_hour;
    hora[4] = timeinfo->tm_min;
    hora[5] = timeinfo->tm_sec;

    printf ( "Current local time and date:\n");
    printf("\tDay: %i, Month: %i, Year: %i, Hour: %i, Min: %i, Sec: %i \n", hora[0], hora[1], hora[2], hora[3], hora[4], hora[5]);

    return 0;
}
