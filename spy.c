#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

static int lines;

void printCurrentFile(char *fileHandler) {
    for(int i = 0; i < 82*lines; i++){ 
        if(i%82 == 0) { 
            putchar('\n'); 
        } 
        printf("%c", *(fileHandler+i));         
    }
}

void printCurrentProcesses(int *processesHandler, int type) {
    for(int i = 1; i < 40001; i += 4) {
        if(processesHandler[i + 1] == type) {
            switch(processesHandler[i + 2]) {
                case 1:
                    printf("PID: %i, State: %s\n", processesHandler[i], "ACTIVE");
                    break;
                case 2:
                    printf("PID: %i, State: %s\n", processesHandler[i], "SLEEPING");
                    break;
                case 3:
                    printf("PID: %i, State: %s\n", processesHandler[i], "BLOCKED");
                    break;
                default:
                    break;
            }
        } 
    }
}

//Get the ID number of a shared memory segment, needed to get the address
int getSharedMemorySegment(key_t key, int size){
    int shmid;
    if((shmid = shmget(key, size, 0666)) < 0){
        return (-1);
    }
    return (shmid);
}

//Attach the process to a shared memory segment
char *attachCharSharedMemorySegment(key_t key, int size){
    char *shm;
    int ID = getSharedMemorySegment(key, size);
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
    int ID = getSharedMemorySegment(key, size); 
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

int main(int argc, char *argv[]){

    //Input del usuario.
    char dump[100];
    int modo;

    //Shared memory keys
    key_t readersK = 5680;
    key_t selfishK = 5681;
    key_t writerK = 5684;
    key_t fileK = 5685;
    key_t processesK = 5686;
    key_t linesK = 5687;

    //Shared memory locations 
    char *fileSHM, *fileHandler; 
 
    int *readersSHM, *readersHandler; 
    int *selfishSHM, *selfishHandler; 
    int *writerSHM, *writerHandler; 
    int *processesSHM, *processesHandler;
    int *linesSHM, *linesHandler;

    linesSHM = attachSharedMemorySegment(linesK, sizeof(int));
    if(linesSHM == NULL){
        perror("ERROR: Couldn't get shared memory for LINES.");
        exit(1);
    }
    linesHandler = linesSHM;
    lines = (int) *linesHandler;

    readersSHM = attachSharedMemorySegment(readersK, sizeof(int)); 
    if(readersSHM == NULL){ 
        perror("ERROR: Couldn't create shared memory for READERS."); 
        exit(1); 
    } 

    selfishSHM = attachSharedMemorySegment(selfishK, sizeof(int)); 
    if(selfishSHM == NULL){ 
        perror("ERROR: Couldn't create shared memory for SELFISH."); 
        exit(1); 
    }

    writerSHM = attachSharedMemorySegment(writerK, sizeof(int)); 
    if(writerSHM == NULL){ 
        perror("ERROR: Couldn't create shared memory for WRITER."); 
        exit(1); 
    }

    fileSHM = attachCharSharedMemorySegment(fileK, sizeof(char) * 82 * lines); 
    if(fileSHM == NULL){ 
        perror("ERROR: Couldn't create shared memory for FILE."); 
        exit(1); 
    }

    processesSHM = attachSharedMemorySegment(processesK, sizeof(int) * 40001); 
    if(processesSHM  == NULL){ 
        perror("ERROR: Couldn't create shared memory for PROCESSES."); 
        exit(1); 
    }

    printf("\nWelcome to the Spy\n\n");
    
    while (1) {
        printf("Things you can do:\n\n");
        printf("\t 1. Spy the current file\n\n");
        printf("\t 2. Spy the Writers\n\n");
        printf("\t 3. Spy the Readers\n\n");
        printf("\t 4. Spy the Selfish Readers\n\n");

        printf("\t 5. Cancel & exit.\n\n");

        if (!fgets(dump, sizeof dump, stdin)){
            printf("\nERROR retrieving input.\n");
        }

        else{
            char *chk;
            
            if ((int) strtol(dump, (char **)NULL, 10)) {
                modo = (int) strtol(dump, (char **)NULL, 10);

                if (modo > 0 && modo < 6) {
                 
                    //Cases
                    switch(modo) {
                        case 1:
                            printf("\n This is the current file:\n\n");
                            fileHandler = fileSHM;
                            printCurrentFile(fileHandler);
                            break;
                        case 2: 
                            printf("\n These are the Writers\n\n");
                            processesHandler = processesSHM;
                            printCurrentProcesses(processesHandler, 1);
                            break;
                        case 3:
                            printf("\n These are the Readers\n\n");
                            processesHandler = processesSHM;
                            printCurrentProcesses(processesHandler, 2);
                            break;
                        case 4:
                            printf("\n These are the Selfish Readers\n\n");
                            processesHandler = processesSHM;
                            printCurrentProcesses(processesHandler, 3);
                            break;
                        case 5:
                            printf("\n Program finished\n\n");
                            exit(1);
                        default:
                            break;
                    }   
                    //break;
                }
            }
            else {
                printf("\n\nEnter a valid option.\n\n"); 
            }

        }
    }

    return 0;

}