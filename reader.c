#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	int readers_amount;

    if( argc != 2 ) {
       	printf("\nERROR: Only 1 parameter expected, with the amount of readers to create. Program ended.\n\n");
    	return 0;
    }

    if (sscanf (argv[1], "%i", &readers_amount)!=1) {
        printf ("\nERROR: <readers_amount> not an integer\n\n");
        return 0;
    }

}
