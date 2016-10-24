#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_t reader;
static pthread_t threads[10000];
static int smSize = sizeof(int);

//Get the ID number of a shared memory segment, needed to get the address
int getSharedMemorySegment(key_t key){
    int shmid;
    if((shmid = shmget(key, smSize, 0666)) < 0){
        return (-1);
    }
    return (shmid);
}

//Attach the process to a shared memory segment
char *attatchSharedMemorySegment(int ID){
    char *shm;
    if((shm = shmat(ID, NULL, 0)) == (char *) -1){
        perror("ERROR: Couldn't attach shared memory segment");
        return NULL;
    }
    return shm;
}

int main(int argc, char *argv[]){

	int readers_amount;
	int read_time;
	int sleep_time;

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

    key_t finishK = 5683;
    key_t writerK = 5684;
    key_t fileK = 5685;
    key_t pIdCounterK = 5686;

    //Shared memory IDs
    int fullLinesID = getSharedMemorySegment(fullLinesK);
    int whiteLinesID = getSharedMemorySegment(whiteLinesK);
    int readersID = getSharedMemorySegment(readersK);
    int selfishID = getSharedMemorySegment(selfishK);
    
    int finishID = getSharedMemorySegment(finishK);
    int writerID = getSharedMemorySegment(writerK);
    int fileID = getSharedMemorySegment(fileK);
    int pIdCunterID = getSharedMemorySegment(pIdCounterK);

    //Shared memory locations
    char *fullLinesSHM, *whiteLinesSHM, *readersSHM, *selfishSHM, *finishSHM,
    *writerSHM, *fileSHM, *pIdCounterSHM;

    //Need to attach the process to Shared Memory segments before creating child threads

    for(int i = 0; i < readers_amount; i++){
        pthread_create(&reader, NULL, beginReading);
        pthread_join(reader, NULL);
    }
}
