#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

static int lines;
static time_t t;
static int selfish_readers_amount; 
static int read_time;
static int sleep_time;
static int type = 3;
static pthread_t threads[10000];

//Shared memory locations 
static char *fileSHM, *fileHandler; 
 
static int *fullLinesSHM, *fullHandler; 
static int *whiteLinesSHM, *whiteHandler; 
static int *readersSHM, *readersHandler; 
static int *selfishSHM, *selfishHandler; 
static int *selfishConsecutivesSHM, *selfishConsecutivesHandler; 
static int *finishSHM, *finishHandler; 
static int *writerSHM, *writerHandler; 
static int *processesSHM, *processesHandler;
static int *linesSHM, *linesHandler;

static sem_t *mainSem;

struct Process{
    int pId;
    int state;
    int line;
    void (*function)();
} process_default = {0, 0, 0, NULL}; 

static struct Process processes[10000] = {0, 0, 0, NULL};

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


void writeLogStolen (int pid, int line, char buffed[]) {
    FILE *results_file;

    char filename[] = "log.txt";
    results_file = fopen(filename, "a");
    
    if (results_file == NULL) { printf("ERROR: Can't open results file\n"); }
    
    time_t rawtime;
    struct tm * timeinfo;


    int timestamp[3];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timestamp[0] = timeinfo->tm_hour; timestamp[1] = timeinfo->tm_min; timestamp[2] = timeinfo->tm_sec;
    
    fprintf(results_file, "SELFISH_READER: PID: %i; Hour: %i, Min: %i, Sec: %i; Line: %i; Stolen: %s", pid, timestamp[0], timestamp[1], timestamp[2], line, buffed);  
    fclose(results_file);
}


void stealMessageTxt (int pid, int line) {
    FILE *results_file;
    FILE *temp;

    char filename[] = "results.txt";
    char tempfilename[] = "temp.txt";

    results_file = fopen(filename, "r");
    temp = fopen(tempfilename, "w+");

    if (results_file == NULL) { printf("ERROR: Can't open results file\n"); }
    
    time_t rawtime;
    struct tm * timeinfo;

    int timestamp[6];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timestamp[0] = timeinfo->tm_mday; timestamp[1] = timeinfo->tm_mon + 1; timestamp[2] = timeinfo->tm_year + 1900; timestamp[3] = timeinfo->tm_hour; timestamp[4] = timeinfo->tm_min; timestamp[5] = timeinfo->tm_sec;

    int count = 0;
    char buf[256];
    char buffed[256];

    while (fgets(buf, sizeof(buf), results_file)){
        if (count == line){ 
            fputs("\n", temp); 
            sprintf(buffed,"%s",buf);
        }

        else { fprintf(temp, "%s", buf); }

        count++;
    }

    if (count < line) { printf("ERROR: specified line not part of the initialized bounds"); }
    int remove_confirm = remove(filename);
    if (remove_confirm != 0) { printf("ERROR: unable to delete the file"); }
    int rename_confirm = rename(tempfilename, filename);
    if (rename_confirm != 0) { printf("ERROR: unable to rename the file"); }
    
    fclose(results_file);
    fclose(temp);

    if (strcmp(buffed,"\n") != 0) {
        writeLogStolen(pid, line, buffed);    
    }
}

void stealMessageSHM(int pid, int line){
    int place = 82 * line;
    for(int i = 0; i < 82; i++){
        fileHandler[place + i] = 0;
    }
}

void *beginSteal(void *data){
    struct Process *process = (struct Process *) data;
    while(1){
        processesHandler = processesSHM;
        fullHandler = fullLinesSHM;
        whiteHandler = whiteLinesSHM;
        readersHandler = readersSHM;
        selfishHandler = selfishSHM;
        selfishConsecutivesHandler = selfishConsecutivesSHM;
        finishHandler = finishSHM;
        writerHandler = writerSHM;
        fileHandler = fileSHM;
        if(*finishHandler == 1){
            break;
        }else{
            if(selfishConsecutivesHandler[0] < 3 && readersHandler[0] == 0 && selfishHandler[0] == 0 && writerHandler[0] == 0){
                printf("Test\n");
                sem_wait(mainSem);
                selfishHandler[0] = 1;
                readersHandler[0] = 0;
                writerHandler[0] = 0;
                selfishConsecutivesHandler[0] += 1;
                processesHandler[(process->pId * 4) + 3] = 1;

                srand((unsigned) time(&t));
                int line = (int) rand() % lines;

                //printf("%d\n", selfishConsecutivesHandler);

                stealMessageTxt(process->pId, line);
                stealMessageSHM(process->pId, line);
                //Sacar de memoria compartida

                sleep(read_time);
                processesHandler[(process->pId * 4) + 3] = 2;
                whiteHandler[0] += 1;
                fullHandler[0] -= 1;
                readersHandler[0] = 0;
                writerHandler[0] = 0;
                selfishHandler[0] = 0;

                sem_post(mainSem);
                sleep(sleep_time);
            }else{
                processesHandler[(process->pId * 4) + 3] = 3;
            }
        }
    }

}

int main(int argc, char *argv[]){
    mainSem = sem_open("/mainSem", 0644);
    if( argc != 4 ) {
       	printf("ERROR: 3 parameters expected: Amount_Of_Readers, Read_Time, Sleep_Time. Program ended.\n\n");
    	return 0;
    }

    if (sscanf (argv[1], "%i", &selfish_readers_amount)!=1) {
        printf ("\nERROR: <selfish_readers_amount> not an integer\n\n");
        return 0;
    }

    if (sscanf (argv[2], "%i", &read_time)!=1) {
        printf ("\nERROR: <read_time> not an integer\n\n");
        return 0;
    }

    if (sscanf (argv[3], "%i", &sleep_time)!=1) {
        printf ("\nERROR: <sleep_time> not an integer\n\n");
        return 0;
    }

    //Shared memory keys
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

    linesSHM = attachSharedMemorySegment(linesK, sizeof(int));
    if(linesSHM == NULL){
        perror("ERROR: Couldn't get shared memory for LINES.");
        exit(1);
    }
    linesHandler = linesSHM;
    lines = (int) *linesHandler;

    fullLinesSHM = attachSharedMemorySegment(fullLinesK, sizeof(int)); 
    if(fullLinesSHM == NULL){ 
        perror("ERROR: Couldn't get shared memory for FULL_LINES."); 
        exit(1); 
    }

    whiteLinesSHM = attachSharedMemorySegment(whiteLinesK, sizeof(int)); 
    if(whiteLinesSHM == NULL){ 
        perror("ERROR: Couldn't get shared memory for WHITE_LINES."); 
        exit(1); 
    }

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

    selfishConsecutivesSHM = attachSharedMemorySegment(selfishConsecutivesK, sizeof(int)); 
    if(selfishConsecutivesSHM == NULL){ 
        perror("ERROR: Couldn't create shared memory for SELFISH_CONSECUTIVES."); 
        exit(1); 
    }

    finishSHM = attachSharedMemorySegment(finishK, sizeof(int)); 
    if(finishSHM == NULL){ 
        perror("ERROR: Couldn't create shared memory for FINISH."); 
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

    ///////////BEGIN
    //Need to attach the process to Shared Memory segments before creating child threads    
    processesHandler = processesSHM;
    fullHandler = fullLinesSHM;
    whiteHandler = whiteLinesSHM;
    readersHandler = readersSHM;
    selfishHandler = selfishSHM;
    selfishConsecutivesHandler = selfishConsecutivesSHM;
    finishHandler = finishSHM;
    writerHandler = writerSHM;
    fileHandler = fileSHM;

    for(int i = 0; i < selfish_readers_amount; i++){
        while(processesHandler[0] != 0) {}
        processesHandler[0] = 1;

        int counter = 1;
        while(processesHandler[(counter * 4) + 1] != 0){
            counter += 4;
            if(counter >= 40000){
                break;
            }
        }

        processesHandler[(counter*4) + 1] = counter;//ID
        processesHandler[(counter*4) + 2] = type;   //Tipo
        processesHandler[(counter*4) + 3] = 0;      //Estado
        processesHandler[(counter*4) + 4] = 0;      //Linea
        processesHandler[0] = 0;

        processes[i].pId = counter;
        pthread_create(&(threads[i]), NULL, &beginSteal, &(processes[i]));
    }

    for(int i = 0; i < selfish_readers_amount; i++){
        pthread_join(threads[i], NULL);
    }
}