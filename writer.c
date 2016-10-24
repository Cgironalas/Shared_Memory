#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static FILE *results_file;
static FILE *temp;

void writeMessage (int pid, int line) {
    
    int line_to_write = line; //largo en chars + 1 (el \n)

    results_file = fopen("results.txt", "r");
    temp = fopen("temp.txt", "w+");

    if (results_file == NULL) {
        printf("ERROR: Can't open results file\n");
    }
    
    time_t rawtime;
    struct tm * timeinfo;

    int timestamp[6];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timestamp[0] = timeinfo->tm_mday; timestamp[1] = timeinfo->tm_mon + 1; timestamp[2] = timeinfo->tm_year + 1900; timestamp[3] = timeinfo->tm_hour; timestamp[4] = timeinfo->tm_min; timestamp[5] = timeinfo->tm_sec;

    int count = 0;
    char buf[256];

    while (fgets(buf, sizeof(buf), results_file)){

        if (count == line_to_write){
            fputs("new new new line  \n", temp);    
        }
        
        else {
            fprintf(temp, "%s", buf);
        }
        count++;
    }


    //printf("\t PID: %i, Day: %i, Month: %i, Year: %i, Hour: %i, Min: %i, Sec: %i \n", pid, timestamp[0], timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5]);    

    //char str[] = "This is tutorialspoint.com";
    //fwrite(str , 1 , sizeof(str) , results_file );
    
    //fprintf(results_file,"\n\t PID: %i, Day: %i, Month: %i, Year: %i, Hour: %i, Min: %i, Sec: %i \n", pid, timestamp[0], timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5]);
    fclose(results_file);
    fclose(temp);
}


int main(int argc, char *argv[]){

	int writers_amount; 
	int write_time;
	int sleep_time;

    if( argc != 4 ) {
       	printf("\nERROR: 3 parameters expected: Amount_Of_Writers, Write_Time, Sleep_time to create. Program ended.\n\n");
    	return 0;
    }

    if (sscanf (argv[1], "%i", &writers_amount)!=1) {
        printf ("\nERROR: <writers_amount> not an integer\n\n");
        return 0;
    }

    if (sscanf (argv[2], "%i", &write_time)!=1) {
        printf ("\nERROR: <write_time> not an integer\n\n");
        return 0;
    }

    if (sscanf (argv[3], "%i", &sleep_time)!=1) {
        printf ("\nERROR: <sleep_time> not an integer\n\n");
        return 0;
    }

    //Printea y escribe en archivo.
    writeMessage(10,3);

    

}