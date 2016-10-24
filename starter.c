#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

static int smSize = sizeof(int);

int createSharedMemorySegment(key_t key){
	int shmid;
	if((shmid = shmget(key, smSize, IPC_CREAT | IPC_EXCL | 0666)) < 0){
		return (-1);
	}
	return (shmid);
}

int *attatchSharedMemorySegment(key_t key){
    int *shm;
    int ID = createSharedMemorySegment(key);
    if(ID != -1){
        if((shm = shmat(ID, NULL, 0)) == (int *) -1){
            return NULL;
        }else{
            return shm;
        }
    }else{
        return NULL;
    }
}

int main(int argc, char *argv[]){
 	int lines; 

    if( argc != 2 ) {
       	printf("\nERROR: Only 1 parameter expected, with the shared memory size. Program ended.\n\n");
    	return 0;
    }

    if (sscanf (argv[1], "%i", &lines)!=1) {
        printf ("\nERROR: <shared_mem_size> not an integer\n\n");
        return 0;
    }

	//Shared Memory keys
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

	int *fullLinesSHM, *whiteLinesSHM, *readersSHM, *selfishSHM, *selfishConsecutivesSHM,
	*finishSHM, *writerSHM, *fileSHM, *pIdCounterSHM, *linesSHM;
	int *s;

    fullLinesSHM = attatchSharedMemorySegment(fullLinesK);
    if(fullLinesSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for FULL_LINES.");
    	exit(1);
	}else{
		s = fullLinesSHM;
		*s = 0;
	}

    whiteLinesSHM = attatchSharedMemorySegment(whiteLinesK);
    if(whiteLinesSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for WHITE_LINES.");
    	exit(1);
	}else{
		s = whiteLinesSHM;
		*s = lines;
	}

    readersSHM = attatchSharedMemorySegment(readersK);
    if(readersSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for READERS.");
    	exit(1);
	}else{
		s = readersSHM;
		*s = 0;
	}

    selfishSHM = attatchSharedMemorySegment(selfishK);
    if(selfishSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for SELFISH.");
    	exit(1);
	}else{
		s = selfishSHM;
		*s = 0;
	}

    selfishConsecutivesSHM = attatchSharedMemorySegment(selfishConsecutivesK);
    if(selfishConsecutivesSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for SELFISH_CONSECUTIVES.");
    	exit(1);
	}else{
		s = selfishConsecutivesSHM;
		*s = 0;
	}

    finishSHM = attatchSharedMemorySegment(finishK);
    if(finishSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for FINISH.");
    	exit(1);
	}else{
		s = finishSHM;
		*s = 0;
	}

    writerSHM = attatchSharedMemorySegment(writerK);
    if(writerSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for WRITER.");
    	exit(1);
	}else{
		s = writerSHM;
		*s = 0;
	}

    fileSHM = attatchSharedMemorySegment(fileK);
   	if(fileSHM == NULL){
   		perror("ERROR: Couldn't create shared memory for FILE.");
    	exit(1);
	}else{
		s = fileSHM;
		*s = 0;
	}

   	pIdCounterSHM = attatchSharedMemorySegment(pIdCounterK);
    if(pIdCounterSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for ID_COUNTER.");
    	exit(1);
	}else{
		s = pIdCounterSHM;
		*s = 0;
	}

    linesSHM = attatchSharedMemorySegment(linesK);
	if(linesSHM == NULL){
		perror("ERROR: Couldn't create shared memory for LINES.");
    	exit(1);
	}else{
		s = linesSHM;
		*s = 0;
	}

	printf("All shared memory spaces successfully created.\n");

	exit(0);
}