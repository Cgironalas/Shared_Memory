#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

static int rtrn;
static char *shm, *s;
static struct shmid_ds shmid_ds;

int getSharedMemorySegment(key_t key){
	int shmid;
	if((shmid = shmget(key, 27, 0666)) < 0){
		return (-1);
	}
	return (shmid);
}

int deleteSharedMemorySegment(int ID){
	//Esto marca la memoria compartida para morir, pero todos los procesos tienen que estar como deattached para que se borre.
	if((rtrn = shmctl(ID, IPC_RMID, &shmid_ds)) < 0){
		perror("ERROR: Failed deleting shared memory segment");
		return(-1);
	}else{
		printf("Shared memory segment successfully deleted\n");
		return rtrn;
	}
}

char *attatchSharedMemorySegment(int ID){
	//Se acopla a la memoria compartida
	char *shm;
	if((shm = shmat(ID, NULL, 0)) == (char *) -1){
		perror("ERROR: Couldn't attach shared memory segment");
		return NULL;
	}//else{
	//	deleteSharedMemorySegment(fullLinesID);
	//}
	return shm;
}

int main(int argc, char *arcgv[]){	
	//Keys de los semaforos
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

	int smSize = sizeof(int);

	//Ids de los semaforos
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

	//En eso se va a guardar la direccion de memorias compartidas
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