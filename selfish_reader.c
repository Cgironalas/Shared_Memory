#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void writeLogStolen (int pid, int line, char buffed[]) {
    FILE *results_file;

    char filename[] = "log.txt";
    results_file = fopen(filename, "a");
    
    if (results_file == NULL) { printf("ERROR: Can't open results file\n"); }
    
    time_t rawtime;
    struct tm * timeinfo;


    int timestamp[3];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timestamp[0] = timeinfo->tm_hour; timestamp[1] = timeinfo->tm_min; timestamp[2] = timeinfo->tm_sec;
    
    fprintf(results_file, "SELFISH_READER: PID: %i; Hour: %i, Min: %i, Sec: %i; Line: %i; Stolen: %s", pid, timestamp[0], timestamp[1], timestamp[2], line, buffed);  
    fclose(results_file);
}


void stealMessageTxt (int pid, int line) {
    FILE *results_file;
    FILE *temp;

    char filename[] = "results.txt";
    char tempfilename[] = "temp.txt";

    results_file = fopen(filename, "r");
    temp = fopen(tempfilename, "w+");

    if (results_file == NULL) { printf("ERROR: Can't open results file\n"); }
    
    time_t rawtime;
    struct tm * timeinfo;

    int timestamp[6];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timestamp[0] = timeinfo->tm_mday; timestamp[1] = timeinfo->tm_mon + 1; timestamp[2] = timeinfo->tm_year + 1900; timestamp[3] = timeinfo->tm_hour; timestamp[4] = timeinfo->tm_min; timestamp[5] = timeinfo->tm_sec;

    int count = 0;
    char buf[256];
    char buffed[256];

    while (fgets(buf, sizeof(buf), results_file)){
        if (count >= line){ 
            fputs("\n", temp); 
            sprintf(buffed,"%s",buf);
        }

        else { fprintf(temp, "%s", buf); }

        count++;
    }

    if (count < line) { printf("ERROR: specified line not part of the initialized bounds"); }
    int remove_confirm = remove(filename);
    if (remove_confirm != 0) { printf("ERROR: unable to delete the file"); }
    int rename_confirm = rename(tempfilename, filename);
    if (rename_confirm != 0) { printf("ERROR: unable to rename the file"); }
    
    fclose(results_file);
    fclose(temp);
    writeLogStolen(pid, line, buffed);
}

int main(int argc, char *argv[]){

	int selfish_readers_amount; 
	int read_time;
	int sleep_time;


    if( argc != 4 ) {
       	printf("ERROR: 3 parameters expected: Amount_Of_Readers, Read_Time, Sleep_Time. Program ended.\n\n");
    	return 0;
    }

    if (sscanf (argv[1], "%i", &selfish_readers_amount)!=1) {
        printf ("\nERROR: <selfish_readers_amount> not an integer\n\n");
        return 0;
    }

    if (sscanf (argv[2], "%i", &read_time)!=1) {
        printf ("\nERROR: <read_time> not an integer\n\n");
        return 0;
    }

    if (sscanf (argv[3], "%i", &sleep_time)!=1) {
        printf ("\nERROR: <sleep_time> not an integer\n\n");
        return 0;
    }

    stealMessageTxt(2, 20);

}