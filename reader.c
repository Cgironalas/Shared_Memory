#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int pId = 0;
static FILE *results_file;
static pthread_t reader;
static pthread_t threads[10000];
static int smSize = sizeof(int);

static int readers_amount;
static int read_time;
static int sleep_time;

struct Process{
    int pId;
    int block;
    int type;
    void (*function)();
} process_dafault = {0, 0, NULL};


//Get the ID number of a shared memory segment, needed to get the address
int getSharedMemorySegment(key_t key){
    int shmid;
    if((shmid = shmget(key, smSize, 0666)) < 0){
        return (-1);
    }
    return (shmid);
}

//Attach the process to a shared memory segment
char *attatchSharedMemorySegment(key_t key){
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

void *beginReading(void *data){

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


int main(int argc, char *argv[]){

    if( argc != 4 ) {
       	printf("\nERROR: 3 parameters expected: Amount_Of_Readers, Read_Time, Sleep_Time. Program ended.\n\n");
    	return 0;
    }

    if (sscanf (argv[1], "%i", &readers_amount)!=1) {
        printf ("\nERROR: <readers_amount> not an integer\n\n");
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

    //Shared memory keys
    key_t fullLinesK = 5678;
    key_t whiteLinesK = 5679;
    key_t readersK = 5680;
    key_t selfishK = 5681;
    key_t selfishConsecutivesK = 5682;
    key_t finishK = 5683;
    key_t writerK = 5684;
    key_t fileK = 5685;
    key_t pIdCounterK = 5686;
    key_t linesK = 5687;

    //Shared memory IDs
    /*
    int fullLinesID = getSharedMemorySegment(fullLinesK);
    int whiteLinesID = getSharedMemorySegment(whiteLinesK);
    int readersID = getSharedMemorySegment(readersK);
    int selfishID = getSharedMemorySegment(selfishK);
    
    int finishID = getSharedMemorySegment(finishK);
    int writerID = getSharedMemorySegment(writerK);
    int fileID = getSharedMemorySegment(fileK);
    
    int linesID = getSharedMemorySegment(linesK);
    */

    //Shared memory locations
    char *fullLinesSHM, *whiteLinesSHM, *readersSHM, *selfishSHM, *selfishConsecutivesSHM, *finishSHM,
    *writerSHM, *fileSHM, *pIdCounterSHM, *linesSHM;

    //Need to attach the process to Shared Memory segments before creating child threads
    /*
    fullLinesSHM = attatchSharedMemorySegment(fullLinesID);
    whiteLinesSHM = attatchSharedMemorySegment(whiteLinesID);
    readersSHM = attatchSharedMemorySegment(readersID);
    selfishSHM = attatchSharedMemorySegment(selfishID);
    
    finishSHM = attatchSharedMemorySegment(finishID);
    writerSHM = attatchSharedMemorySegment(writerID);
    fileSHM = attatchSharedMemorySegment(fileID);
    
    linesSHM = attatchSharedMemorySegment(linesID);
    */

    fullLinesSHM = attatchSharedMemorySegment(fullLinesK);
    whiteLinesSHM = attatchSharedMemorySegment(whiteLinesK);
    readersSHM = attatchSharedMemorySegment(readersK);
    selfishSHM = attatchSharedMemorySegment(selfishK);
    
    finishSHM = attatchSharedMemorySegment(finishK);
    writerSHM = attatchSharedMemorySegment(writerK);
    fileSHM = attatchSharedMemorySegment(fileK);
    
    linesSHM = attatchSharedMemorySegment(linesK);

    struct Process process;

    for(int i = 0; i < readers_amount; i++){
        pthread_create(&reader, NULL, beginReading, &process);
        pthread_join(reader, NULL);
    }
}
