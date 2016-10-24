#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int createSharedMemorySegment(key_t key){
	int shmid;
	if((shmid = shmget(key, 27, IPC_CREAT | IPC_EXCL | 0666)) < 0){
		return (-1);
	}
	return (shmid);
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

	//Semaphore keys
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

	//Semaphore ids
	int fullLinesID = createSharedMemorySegment(fullLinesK);
	int whiteLinesID = createSharedMemorySegment(whiteLinesK);
	int readersID = createSharedMemorySegment(readersK);
	int selfishID = createSharedMemorySegment(selfishK);
	int selfishConsecutivesID = createSharedMemorySegment(selfishConsecutivesK);
	int finishID = createSharedMemorySegment(finishK);
	int writerID = createSharedMemorySegment(writerK);
	int fileID = createSharedMemorySegment(fileK);
	int pIdCunterID = createSharedMemorySegment(pIdCounterK);
	int linesID = createSharedMemorySegment(linesK);

	if(fullLinesID == -1 || whiteLinesID == -1 || readersID == -1 || selfishID == -1 ||
		selfishConsecutivesID == -1 || finishID == -1 || writerID == -1 || fileID == -1 ||
		pIdCunterID == -1 || linesID == -1){
		perror("Couldn't initialize all shared memory segments");
		
		//if(finishID > 0){
		//	if((shm = shmat(finishID, NULL, 0)) == (char *) -1){
		//		perror("No se logro acoplar al segmento de memoria compartida");
		//		exit(1);
		//	}
		//	s = shm;		//Setea en s la direccion de inicio de la memoria compartida
		//	s* = '*';		//Escribe un '*' en el primer campo de memoria compartida
		//}
		exit(1);
	}

	printf("All shared memory spaces successfully created.\n");

	exit(0);
}