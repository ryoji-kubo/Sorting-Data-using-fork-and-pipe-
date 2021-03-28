#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "coord.h"
#include "taxpayer.h"
#include "myhie.h"

int sig1Counter = 0;    //Global var which will count how many SIGUSR1 received

int main(int argc, char *argv[]){

    timeval tv1; //record starting time
    gettimeofday(&tv1,NULL);

    signal(SIGUSR1,signalHandler);  /* establish disposition of the signal SIGUSR1 */
    signal(SIGUSR2,signalHandler2);  /* establish disposition of the signal SIGUSR2 */

    /*==================COMMAND LINE CONTROL=============*/
    int SIZE = 100;         //buffer size
    char inputFile[SIZE];     //the input filename
    char outputFile[SIZE];   //the output filename
    int numSort;        //number of sorters to be sorted
    char random[SIZE];      //if it is random
    strcpy(random,"notRandom");        // instructs the program to have workers work on “random ranges” or not-equally-sized batches of data-records, default: not random
    char attributeNum[SIZE]; //a valid numeric-id that designates the field on which sorting is to be carried out (0, 3, 4, or 5)
    char order[SIZE]; //ascending is 0 and descending is 1

    if (argc != 11 && argc != 12){        //checks if the number of input is valid
        fprintf(stderr, "Invalid Input\n");
        exit(EXIT_SUCCESS);
    }

    int inputCounter = 0;   //used to check if the input is vaild

    for (int i = 0; i<argc; i++){   //
        if (strcmp(argv[i],"-i")==0){
            strcpy(inputFile, argv[i+1]);      //get input filename
            inputCounter++;
        }
        else if (strcmp(argv[i],"-k")==0){
            numSort = atoi(argv[i+1]);      //get number of sorters
            inputCounter++;
        }
        else if (strcmp(argv[i],"-r")==0){
            strcpy(random, "yesRandom");      //get random is on (optional so no inputCounter increment
        }
        else if (strcmp(argv[i],"-a")==0 && (strcmp(argv[i+1],"0")==0||strcmp(argv[i+1],"3")==0||strcmp(argv[i+1],"4")==0||strcmp(argv[i+1],"5")==0)){    //checks if the attribute number is appropritate
            strcpy(attributeNum,argv[i+1]);      //get the attribute number
            inputCounter++;
        }
        else if (strcmp(argv[i],"-o")==0){        
            if (strcmp(argv[i+1],"a")==0 || strcmp(argv[i+1],"d")==0){       //ascending order
                strcpy(order,argv[i+1]);
                inputCounter++; 
            }
        }
        else if (strcmp(argv[i],"-s")==0){
            strcpy(outputFile, argv[i+1]);      //get output filename
            inputCounter++;
        }
    }
    if (inputCounter != 5){     //if the input is invalid
        fprintf(stderr, "Invalid Input\n");
        exit(EXIT_SUCCESS);
    }
    /*==========================================================================*/

    /*==================CREATING COORD NODE===================================*/
    pid_t pid;      //holds the process id
    switch (pid = fork()) {         //fork() and check switch if in parent (root) process or child (coord) process
    case -1:
        // On error fork() returns -1.
        perror("fork failed");
        exit(EXIT_FAILURE); 
    case 0:
        // On success fork() returns 0 in the child.
        coord(inputFile, outputFile, numSort, attributeNum, random, order);     //go into the coord process
    default:
        // On success fork() returns the pid of the child to the parent.
        root(pid, tv1);                  //go into the root process
    }
    /*==========================================================================*/
    
    return 0;
}

void root(pid_t pid, timeval tv1){
    int status;

    printf("Sorting...\n");
    //wait for coord process
    waitpid(pid , &status , 0);

    printf("**Received %d SIGUSR1 from Sorters**\n", sig1Counter);

    timeval tv2; //record starting time
    gettimeofday(&tv2,NULL);//record finishing time
    long t = (tv2.tv_sec*1e6 + tv2.tv_usec) - (tv1.tv_sec*1e6 + tv1.tv_usec);
    printf("---ENTIRE PROCESS TOOK %ld MICROSECONDS---\n", t);

    exit(EXIT_SUCCESS); //exit application
}

void signalHandler(int signal)
{
	sig1Counter++;  //increment global var
}

void signalHandler2(int signal)
{
	printf("**Caught 1 SIGUSR2 from Merger, Sorting completed**\n"); //caught!
}