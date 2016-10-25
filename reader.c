#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static int pId = 0;
static FILE *results_file;
static pthread_t reader;
static pthread_t threads[10000];
static int smSize = sizeof(int);

static int readers_amount;
static int read_time;
static int sleep_time;

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

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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

void writeLogRead (int pid, int line, char buffed[]) {
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
    
    fprintf(results_file, "READER: PID: %i; Hour: %i, Min: %i, Sec: %i; Line: %i; Read: %s", pid, timestamp[0], timestamp[1], timestamp[2], line, buffed);  
    fclose(results_file);
}

void readLine (int pid, int line) {
    FILE *results_file;
    
    char filename[] = "results.txt";
    
    results_file = fopen(filename, "r");
    
    if (results_file == NULL) { printf("ERROR: Can't open results file\n"); }

    int count = 0;
    char buf[256];
    int foundsomething = 0;

    while (fgets(buf, sizeof(buf), results_file)){

        if (count >= line && strcmp(buf,"\n") != 0){ 
            line = count+1;
            writeLogRead(pid, line, buf);
            printf("%s\n", buf);

            count++;
            foundsomething = 1;
            break;
        }
        count++;
    }

    fclose(results_file);

    if (count < line){ printf("ERROR: Couldn't access such line\n"); }
    
    if (foundsomething == 0){
        readLine (pid, 0);
    }
}

//Get the ID number of a shared memory segment, needed to get the address
int getSharedMemorySegment(key_t key){
    int shmid;
    if((shmid = shmget(key, smSize, 0666)) < 0){
        return (-1);
    }
    return (shmid);
}

//Attach the process to a shared memory segment
char *attatchSharedMemorySegment(key_t key){
    char *shm;
    int ID = getSharedMemorySegment(key);
    if(ID != -1){
        if((shm = shmat(ID, NULL, 0)) == (char *) -1){
            perror("ERROR: Couldn't attach shared memory segment\n");
            return NULL;
        }else{
            return shm;
        }
    }else{
        perror("ERROR: Couldn't create shared memory segment.\n");
        return NULL;
    }
}

void *beginReading(void *data){
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
            pthread_mutex_lock(&lock);
            
            if(1 == 1){

                readersHandler[0] += 1;
                selfishConsecutivesHandler[0] = 0;

                processesHandler[(process->pId * 4) + 3] = 1;   //Estado activo            

                readLine(process->pId, process->line);
                //Leer memoria compartida
                process->line = processesHandler[(process->pId * 4) + 4];

                sleep(read_time);
                processesHandler[(process->pId * 4) + 3] = 2;  //Estado inactivo

                writerHandler[0] = 0;
                sleep(sleep_time);

            }else{
                processesHandler[(process->pId * 4) + 3] = 3;  //Estado bloqueado
            }

            pthread_mutex_unlock(&lock);

        }
    }
}

void p(int s, struct Process p) {
    while (s <= 0){
        p.block = 1;
    }
    s -= 1;
}

void v(int s, struct Process p ) {
    while (s < 0){
        p.block = 0;
    }
    s += 1;
}


int main(int argc, char *argv[]){

    if( argc != 4 ) {
       	printf("\nERROR: 3 parameters expected: Amount_Of_Readers, Read_Time, Sleep_Time. Program ended.\n\n");
    	return 0;
    }

    if (sscanf (argv[1], "%i", &readers_amount)!=1) {
        printf ("\nERROR: <readers_amount> not an integer\n\n");
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

    read_time = argv[2];
    sleep_time = argv[3];
    readers_amount = argv[1];

    //Shared memory keys
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
    /*
    int fullLinesID = getSharedMemorySegment(fullLinesK);
    int whiteLinesID = getSharedMemorySegment(whiteLinesK);
    int readersID = getSharedMemorySegment(readersK);
    int selfishID = getSharedMemorySegment(selfishK);
    
    int finishID = getSharedMemorySegment(finishK);
    int writerID = getSharedMemorySegment(writerK);
    int fileID = getSharedMemorySegment(fileK);
    
    int linesID = getSharedMemorySegment(linesK);
    */

    //Shared memory locations
    char *fullLinesSHM, *whiteLinesSHM, *readersSHM, *selfishSHM, *selfishConsecutivesSHM, *finishSHM,
    *writerSHM, *fileSHM, *pIdCounterSHM, *linesSHM;

    //Need to attach the process to Shared Memory segments before creating child threads
    /*
    fullLinesSHM = attatchSharedMemorySegment(fullLinesID);
    whiteLinesSHM = attatchSharedMemorySegment(whiteLinesID);
    readersSHM = attatchSharedMemorySegment(readersID);
    selfishSHM = attatchSharedMemorySegment(selfishID);
    
    finishSHM = attatchSharedMemorySegment(finishID);
    writerSHM = attatchSharedMemorySegment(writerID);
    fileSHM = attatchSharedMemorySegment(fileID);
    
    linesSHM = attatchSharedMemorySegment(linesID);
    */

    fullLinesSHM = attatchSharedMemorySegment(fullLinesK);
    whiteLinesSHM = attatchSharedMemorySegment(whiteLinesK);
    readersSHM = attatchSharedMemorySegment(readersK);
    selfishSHM = attatchSharedMemorySegment(selfishK);
    
    finishSHM = attatchSharedMemorySegment(finishK);
    writerSHM = attatchSharedMemorySegment(writerK);
    fileSHM = attatchSharedMemorySegment(fileK);
    
    //linesSHM = attatchSharedMemorySegment(linesK);

    struct Process process;

    for(int i = 0; i < readers_amount; i++){
        pthread_create(&reader, NULL, beginReading, &process);
        pthread_join(reader, NULL);
    }

    readLine(2, 22);
}
