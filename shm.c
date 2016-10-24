#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

//Meter aqui todo lo de manejo de memoria compartida, y hacer include en  los demas archivos

int createSharedMemorySegment(key_t key){
	int shmid;
	if((shmid = shmget(key, 27, IPC_CREAT | IPC_EXCL | 0666)) < 0){
		return (-1);
	}
	return (shmid);
}