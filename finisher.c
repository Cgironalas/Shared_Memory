#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

static int rtrn;
static struct shmid_ds shmid_ds;
static sem_t *mainSem;
static sem_t *readerSem;

//Get the ID number of a shared memory segment, needed to get the address
int getSharedMemorySegment(key_t key, int size){
	int shmid;
	if((shmid = shmget(key, size, 0666)) < 0){
		return (-1);
	}
	return (shmid);
}

char *attachCharSharedMemorySegment(int ID){
	char *shm;
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

//Attach the process to a shared memory segment
int *attatchSharedMemorySegment(int ID){
    int *shm;
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

//Call for a shared memory segment to be deleted, needs for all other processes to detach from it, then it deletes
int deleteSharedMemorySegment(int ID){
	if((rtrn = shmctl(ID, IPC_RMID, &shmid_ds)) < 0){
		perror("ERROR: Failed deleting shared memory segment");
		return(-1);
	}else{
		printf("Shared memory segment successfully deleted\n");
		return rtrn;
	}
}

int main(int argc, char *arcgv[]){
	mainSem = sem_open("/mainSem", 0644);
    readerSem = sem_open("/readerSem", 0644);

    sem_unlink("/mainSem");
    sem_unlink("/readerSem");

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

	//Shared Memory IDs
	int fullLinesID = getSharedMemorySegment(fullLinesK, sizeof(int));
	int whiteLinesID = getSharedMemorySegment(whiteLinesK, sizeof(int));
	int readersID = getSharedMemorySegment(readersK, sizeof(int));
	int selfishID = getSharedMemorySegment(selfishK, sizeof(int));
	int selfishConsecutivesID = getSharedMemorySegment(selfishConsecutivesK, sizeof(int));
	int finishID = getSharedMemorySegment(finishK, sizeof(int));
	int writerID = getSharedMemorySegment(writerK, sizeof(int));
	int processesID = getSharedMemorySegment(processesK, sizeof(int) * 40001);
	int linesID = getSharedMemorySegment(linesK, sizeof(int));

	//Shared memory location
	int *shm;
	char *shm2;

	int lines;

	shm = attatchSharedMemorySegment(finishID);
	//printf("Needs to finish: %d\n", *shm);
	if(shm != NULL){
		*shm = 1;
		sleep(10);
		deleteSharedMemorySegment(finishID);
	}else{
		perror("ERROR: Attaching to the shared memory for FINISH");
	}
	
	//Si se consiguio el ID del semaforo se setea el lugar de memoria correspondiente
	shm = attatchSharedMemorySegment(linesID);
	//printf("Cantidad de lineas del archivo: %d\n", *shm);
	if(shm != NULL){
		lines = *shm;
		deleteSharedMemorySegment(linesID);
	}else{
		lines = 0;
		perror("ERROR: Attaching to the shared memory for FULL_LINES");
	}

	shm = attatchSharedMemorySegment(fullLinesID);
	//printf("Full Lines: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(fullLinesID);
	}else{
		perror("ERROR: Attaching to the shared memory for FULL_LINES");
	}

	shm = attatchSharedMemorySegment(whiteLinesID);
	//printf("White Lines: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(whiteLinesID);
	}else{
		perror("ERROR: Attaching to the shared memory for WHITE_LINES");	
	}
	
	shm = attatchSharedMemorySegment(readersID);
	//printf("Readers active: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(readersID);
	}else{
		perror("ERROR: Attaching to the shared memory for READERS");
	}	

	shm = attatchSharedMemorySegment(selfishID);
	//printf("Selfish readers active: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(selfishID);
	}else{
		perror("ERROR: Attaching to the shared memory for SELFISH");
	}

	shm = attatchSharedMemorySegment(selfishConsecutivesID);
	//printf("Selfish reader consecutives: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(selfishConsecutivesID);
	}else{
		perror("ERROR: Attaching to the shared memory for SELFISH_CONSECUTIVES");
	}

	shm = attatchSharedMemorySegment(writerID);
	//printf("Writers amount: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(writerID);
	}else{
		perror("ERROR: Attaching to the shared memory for WRITER");
	}

	int fileID = getSharedMemorySegment(fileK, sizeof(char) * 82 * lines);
	shm2 = attachCharSharedMemorySegment(fileID);
	//printf("File: %d\n", *shm);
	if(shm2 != NULL){
		deleteSharedMemorySegment(fileID);
	}else{
		perror("ERROR: Attaching to the shared memory for FILE");
	}

	shm = attatchSharedMemorySegment(processesID);
	//printf("Lines in shared memory file: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(processesID);
	}else{
		perror("ERROR: Attaching to the shared memory for PROCESSES");
	}

	int remove_confirm = remove("results.txt");
    if (remove_confirm != 0) { printf("ERROR: unable to delete the file"); }
	remove_confirm = remove("temp.txt");

	exit(0);
}