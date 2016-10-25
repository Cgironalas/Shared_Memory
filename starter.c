#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int createSharedMemorySegment(key_t key, int size){
	int shmid;
	if((shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666)) < 0){
		return (-1);
	}
	return (shmid);
}

char *attachCharSharedMemorySegment(key_t key, int size){
	char *shm;
	int ID = createSharedMemorySegment(key, size);
	if(ID != -1){
		if((shm = shmat(ID, NULL, 0)) == (char *) -1){
			return NULL;
		}else{
			return shm;
		}
	}else{
		return NULL;
	}
}

int *attachSharedMemorySegment(key_t key, int size){
    int *shm;
    int ID = createSharedMemorySegment(key, size);
    if(ID != -1){
        if((shm = shmat(ID, NULL, 0)) == (int *) -1){
            return NULL;
        }else{
            return shm;
        }
    }else{
    	printf("D\n");
        return NULL;
    }
}

void write(int lines) {
    FILE * init;
    FILE * log;
    char buff[256];
    init = fopen("results.txt","w");
    log = fopen("log.txt","w");

    for (int i= 0; i < lines; i++){
        fprintf(init,"%s\n",buff);
    }
    fclose(init);
    fclose(log);
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
    write(lines);
	

	//Shared Memory keys
	key_t fullLinesK = 5678;
	key_t whiteLinesK = 5679;
	key_t readersK = 5680;
	key_t selfishK = 5681;
	key_t selfishConsecutivesK = 5682;
	key_t finishK = 5683;
	key_t writerK = 5684;
	key_t fileK = 5685;
	key_t processesK = 5686;
	key_t linesK = 5687;

	char *fileSHM, *charHandler;
	
	int *fullLinesSHM, *whiteLinesSHM, *readersSHM, *selfishSHM, *selfishConsecutivesSHM,
	*finishSHM, *writerSHM, *processSHM, *linesSHM, *intHandler;

	linesSHM = attachSharedMemorySegment(linesK, sizeof(int));
	if(linesSHM == NULL){
		perror("ERROR: Couldn't create shared memory for LINES.");
    	exit(1);
	}else{
		intHandler = linesSHM;
		*intHandler = lines;
	}

    fullLinesSHM = attachSharedMemorySegment(fullLinesK, sizeof(int));
    if(fullLinesSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for FULL_LINES.");
    	exit(1);
	}else{
		intHandler = fullLinesSHM;
		*intHandler = 0;
	}

    whiteLinesSHM = attachSharedMemorySegment(whiteLinesK, sizeof(int));
    if(whiteLinesSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for WHITE_LINES.");
    	exit(1);
	}else{
		intHandler = whiteLinesSHM;
		*intHandler = lines;
	}

    readersSHM = attachSharedMemorySegment(readersK, sizeof(int));
    if(readersSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for READERS.");
    	exit(1);
	}else{
		intHandler = readersSHM;
		*intHandler = 0;
	}

    selfishSHM = attachSharedMemorySegment(selfishK, sizeof(int));
    if(selfishSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for SELFISH.");
    	exit(1);
	}else{
		intHandler = selfishSHM;
		*intHandler = 0;
	}

    selfishConsecutivesSHM = attachSharedMemorySegment(selfishConsecutivesK, sizeof(int));
    if(selfishConsecutivesSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for SELFISH_CONSECUTIVES.");
    	exit(1);
	}else{
		intHandler = selfishConsecutivesSHM;
		*intHandler = 0;
	}

    finishSHM = attachSharedMemorySegment(finishK, sizeof(int));
    if(finishSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for FINISH.");
    	exit(1);
	}else{
		intHandler = finishSHM;
		*intHandler = 0;
	}

    writerSHM = attachSharedMemorySegment(writerK, sizeof(int));
    if(writerSHM == NULL){
    	perror("ERROR: Couldn't create shared memory for WRITER.");
    	exit(1);
	}else{
		intHandler = writerSHM;
		*intHandler = 0;
	}

    fileSHM = attachCharSharedMemorySegment(fileK, (int) sizeof(char) * 82 * lines);
   	if(fileSHM == NULL){
   		perror("ERROR: Couldn't create shared memory for FILE.");
    	exit(1);
	}else{
		charHandler = fileSHM;
		for(int i = 0; i < sizeof(char) * 82 * lines; i++){
			*charHandler++ = 0;
		}
	}

	processSHM = attachSharedMemorySegment(processesK, (int) sizeof(int) * 40001);		
	if(processSHM  == NULL){
		perror("ERROR: Couldn't create shared memory for PROCESSES.");
    	exit(1);
	}else{
		intHandler = processSHM;
		for(int i = 0; i < 40001; i++){
			intHandler[i] = 0;
		}
	}

	printf("All shared memory spaces successfully created.\n");

	exit(0);
}