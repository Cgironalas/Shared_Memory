#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

static int pId = 0;
static pthread_t reader;
static pthread_t threads[10000];
static int smSize = sizeof(int);

struct Process{
    int pId;
    int block;
    int type;
    int line;
    void (*function)();
} process_default = {0, 0, 0, 0, NULL}; 

//Get the ID number of a shared memory segment, needed to get the address
int getSharedMemorySegment(key_t key){
    int shmid;
    if((shmid = shmget(key, smSize, 0666)) < 0){
        return (-1);
    }
    return (shmid);
}

//Attach the process to a shared memory segment
char *attachSharedMemorySegment(key_t key){
    char *shm;
    int ID = getSharedMemorySegment(key);
    if(ID != -1){
        if((shm = shmat(ID, NULL, 0)) == (char *) -1){
            perror("ERROR: Couldn't attach shared memory segment");
            return NULL;
        }else{
            return shm;
        }
    }else{
        perror("ERROR: Couldn't create shared memory segment.");
        return NULL;
    }
}

void p(int s, struct Process p) {
    while (s <= 0){
        p.block = 1;
    }
    s -= 1;
}

void v(int s, struct Process p ) {
    while (s < 0){
        p.block = 0;
    }
    s += 1;
}

void writeMessageTxt (int pid, int line) {
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

    while (fgets(buf, sizeof(buf), results_file)){
        if (count == line){ 
            sprintf(buf, "PID: %i; Day: %i, Month: %i, Year: %i; Hour: %i, Min: %i, Sec: %i; Line: %i\n", pid, timestamp[0], timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5], line);
            fputs(buf, temp); 
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
}

void writeLogTxt (int pid, int line) {
    FILE *results_file;

    char filename[] = "log.txt";
    results_file = fopen(filename, "a");
    
    if (results_file == NULL) { printf("ERROR: Can't open results file\n"); }
    
    time_t rawtime;
    struct tm * timeinfo;

    int timestamp[6];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timestamp[0] = timeinfo->tm_mday; timestamp[1] = timeinfo->tm_mon + 1; timestamp[2] = timeinfo->tm_year + 1900; timestamp[3] = timeinfo->tm_hour; timestamp[4] = timeinfo->tm_min; timestamp[5] = timeinfo->tm_sec;
    
    fprintf(results_file, "PID: %i; Hour: %i, Min: %i, Sec: %i; Line: %i\n", pid, timestamp[0], timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5], line);  
    fclose(results_file);
}

void *beginReading(void* data){
    struct Process *process = (struct Process *) data;
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

    //Shared memory keys
    key_t fullLinesK = 5678;
    key_t whiteLinesK = 5679;
    key_t readersK = 5680;
    key_t selfishK = 5681;

    key_t finishK = 5683;
    key_t writerK = 5684;
    key_t fileK = 5685;

    key_t linesK = 5687;

    //Shared memory IDs
    /*int fullLinesID = getSharedMemorySegment(fullLinesK);
    int whiteLinesID = getSharedMemorySegment(whiteLinesK);
    int readersID = getSharedMemorySegment(readersK);
    int selfishID = getSharedMemorySegment(selfishK);
    
    int finishID = getSharedMemorySegment(finishK);
    int writerID = getSharedMemorySegment(writerK);
    int fileID = getSharedMemorySegment(fileK);

    int linesID = getSharedMemorySegment(linesK);
    */

    //Shared memory locations
    char *fullLinesSHM, *whiteLinesSHM, *readersSHM, *selfishSHM, *finishSHM,
    *writerSHM, *fileSHM, *linesSHM;

    /*
    fullLinesSHM = attachSharedMemorySegment(fullLinesID);
    whiteLinesSHM = attachSharedMemorySegment(whiteLinesID);
    readersSHM = attachSharedMemorySegment(readersID);
    selfishSHM = attachSharedMemorySegment(selfishID);
    
    finishSHM = attachSharedMemorySegment(finishID);
    writerSHM = attachSharedMemorySegment(writerID);
    fileSHM = attachSharedMemorySegment(fileID);
    
    linesSHM = attachSharedMemorySegment(linesID);
    */

    fullLinesSHM = attachSharedMemorySegment(fullLinesK);
    whiteLinesSHM = attachSharedMemorySegment(whiteLinesK);
    readersSHM = attachSharedMemorySegment(readersK);
    selfishSHM = attachSharedMemorySegment(selfishK);
    
    finishSHM = attachSharedMemorySegment(finishK);
    writerSHM = attachSharedMemorySegment(writerK);
    fileSHM = attachSharedMemorySegment(fileK);
    
    //linesSHM = attachSharedMemorySegment(linesK);

    //Need to attach the process to Shared Memory segments before creating child threads
    struct Process process = process_default;
    
    for(int i = 0; i < writers_amount; i++){
        process.pId = i;
        pthread_create(&reader, NULL, beginReading, &process);
        pthread_join(reader, NULL);
    }

    //Printea y escribe en archivo.
    writeMessageTxt(10,20);
    writeMessageTxt(10,21);
    writeMessageTxt(10,22);
    writeMessageTxt(10,23);
       
}