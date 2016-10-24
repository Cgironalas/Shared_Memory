#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

static int rtrn;
static char *shm, *s;
static struct shmid_ds shmid_ds;
static int smSize = sizeof(int);

//Get the ID number of a shared memory segment, needed to get the address
int getSharedMemorySegment(key_t key){
	int shmid;
	if((shmid = shmget(key, shSize, 0666)) < 0){
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

	//Shared memory IDs
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
	char *shm;
	

	//Si se consiguio el ID del semaforo se setea el lugar de memoria correspondiente
	if(fullLinesID != -1){
		shm = attatchSharedMemorySegment(fullLinesID);
		if(shm != NULL){
			deleteSharedMemorySegment(fullLinesID);
		}
	}
	
	if(whiteLinesID != -1){
		shm = attatchSharedMemorySegment(whiteLinesID);
		if(shm != NULL){
			deleteSharedMemorySegment(whiteLinesID);
		}
	}

	if(readersID != -1){
		shm = attatchSharedMemorySegment(readersID);
		if(shm != NULL){
			deleteSharedMemorySegment(readersID);
		}	
	}

	if(selfishID != -1){
		shm = attatchSharedMemorySegment(selfishID);
		if(shm != NULL){
			deleteSharedMemorySegment(selfishID);
		}
	}

	if(selfishConsecutivesID != -1){
		shm = attatchSharedMemorySegment(selfishConsecutivesID);
		if(shm != NULL){
			deleteSharedMemorySegment(selfishConsecutivesID);
		}
	}

	if(finishID != -1){
		shm = attatchSharedMemorySegment(finishID);
		if(shm != NULL){
			deleteSharedMemorySegment(finishID);
		}
	}

	if(writerID != -1){
		shm = attatchSharedMemorySegment(writerID);
		if(shm != NULL){
			deleteSharedMemorySegment(writerID);
		}
	}

	if(fileID != -1){
		shm = attatchSharedMemorySegment(fileID);
		if(shm != NULL){
			deleteSharedMemorySegment(fileID);
		}
	}

	if(pIdCunterID != -1){
		shm = attatchSharedMemorySegment(pIdCunterID);
		if(shm != NULL){
			deleteSharedMemorySegment(pIdCunterID);
		}
	}

	if(linesID != -1){
		shm = attatchSharedMemorySegment(linesID);
		if(shm != NULL){
			deleteSharedMemorySegment(linesID);
		}
	}
	

	exit(0);
}