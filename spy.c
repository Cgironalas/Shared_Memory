#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	//Input del usuario.
	char dump[100];
	int modo;

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
                            // Code here
                            break;
    					case 2: 
    						printf("\n These are the Writers\n\n");
                            // Code here
                            break;
    					case 3:
    						printf("\n These are the Readers\n\n");
    						// Code here
    						break;
    					case 4:
    						printf("\n These are the Selfish Readers\n\n");
    						// Code here
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