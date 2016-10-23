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

	//Ids de los semaforos
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
		perror("No se logro iniciar todos los segmentos de memoria compartida.");
		
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

	printf("Todos se hicieron bien.\n");

	exit(0);
}