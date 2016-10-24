#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

static int rtrn;
static struct shmid_ds shmid_ds;
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

	//Shared Memory IDs
	int fullLinesID = getSharedMemorySegment(fullLinesK);
	int whiteLinesID = getSharedMemorySegment(whiteLinesK);
	int readersID = getSharedMemorySegment(readersK);
	int selfishID = getSharedMemorySegment(selfishK);
	int selfishConsecutivesID = getSharedMemorySegment(selfishConsecutivesK);
	int finishID = getSharedMemorySegment(finishK);
	int writerID = getSharedMemorySegment(writerK);
	int fileID = getSharedMemorySegment(fileK);
	int pIdCunterID = getSharedMemorySegment(pIdCounterK);
	int linesID = getSharedMemorySegment(linesK);

	//Shared memory location
	int *shm;
	
	//Si se consiguio el ID del semaforo se setea el lugar de memoria correspondiente
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

	shm = attatchSharedMemorySegment(finishID);
	//printf("Needs to finish: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(finishID);
	}else{
		perror("ERROR: Attaching to the shared memory for FINISH");
	}

	shm = attatchSharedMemorySegment(writerID);
	//printf("Writers amount: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(writerID);
	}else{
		perror("ERROR: Attaching to the shared memory for WRITER");
	}

	shm = attatchSharedMemorySegment(fileID);
	//printf("File: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(fileID);
	}else{
		perror("ERROR: Attaching to the shared memory for FILE");
	}

	shm = attatchSharedMemorySegment(pIdCunterID);
	//printf("ID counter: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(pIdCunterID);
	}else{
		perror("ERROR: Attaching to the shared memory for ID_COUNTER");
	}

	shm = attatchSharedMemorySegment(linesID);
	//printf("Lines in shared memory file: %d\n", *shm);
	if(shm != NULL){
		deleteSharedMemorySegment(linesID);
	}else{
		perror("ERROR: Attaching to the shared memory for LINES");
	}
	
	exit(0);
}