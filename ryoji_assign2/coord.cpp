#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<string>
#include <unistd.h>
#include <sys/wait.h> 
#include <ctime>

#include "coord.h"
#include "taxpayer.h"
#include "sort.h"
#include "merger.h"

using namespace std;

void coord(char inputFile[], char outputFile[], int numSort, char* attributeNum, char* random, char* order){

    // //===================READING THE TOTAL NUMBER OF LINES FROM THE INPUT FILE=========================//
    int arraySize = 0;      //counts to determine the array size (total numbeer of entry)

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(inputFile, "r");              //open to read
    if (fp == NULL){                        //if no content
        printf("READING INPUT FILE ERROR\n");
        exit(EXIT_FAILURE);
    }
    char c;
    for (c = getc(fp); c != EOF; c = getc(fp)){     //https://www.geeksforgeeks.org/c-program-count-number-lines-file/
        if (c == '\n'){ // Increment count if this character is newline 
            arraySize++; 
        }
    }
    fclose(fp);

    //==========================================================================================================//


    /*====================ASSIGNING THE RANGE FOR EACH SORTER======================*/
    int rangeArray[numSort];    //a rangeArray to keep the range for each sorter node.
    if (strcmp(random, "yesRandom")==0){        //if the range is randomly assigned
        int remaining = arraySize;  //checks how many remaining!
        srand(time(NULL));  //initialization
        for (int i = 0; i<numSort; i++){
            rangeArray[i] = 1;      //make sure each sorter has at least one element
        }
        remaining = remaining - numSort;        //decrease the remaning size
        for (int i = 0; i<numSort; i++){        //for each of the sorter node
            if (i+1==numSort){      //for the last sorter node
                rangeArray[i]+=remaining;    //assign whatever is left
            }
            else{       //for all the nodes other than the last
                int r = rand()%remaining;       //randomly pick an int number in the range from 0 to remaining
                rangeArray[i] += r;              //assign the randomly picked number to the sorter node (but add it to the 1 element we gave at the beginning)
                remaining = remaining - r;      //change the remaining value
            }
        }
    }
    else{               //if the range is NOT randomly assigned
        if (numSort<=arraySize){    //if there is less sorters than the data entry
            int range = arraySize/numSort;  //integer division to assign approx. equal size
            for (int i = 0; i<numSort; i++){    //for each sorter node
                if (i+1==numSort){          //if last node
                    rangeArray[i]=range+arraySize%numSort;        //add the remainder
                }
                else{           //if not the last node
                    rangeArray[i]=range;    //assign the equally distributed range
                }
            }
        }
        else{       //if there are more sorters than data entry
            int diff = numSort - arraySize;     //get the difference
            for (int i = 0; i<diff; i++){       //assign 0 to prevent overflow
                rangeArray[i] = 0;
            }
            for (int i = diff; i<numSort; i++){     //everyone else gets 1
                rangeArray[i] = 1;
            }
        }

        // int range = arraySize/numSort;  //integer division to assign approx. equal size
        // for (int i = 0; i<numSort; i++){    //for each sorter node
        //     if (i+1==numSort){          //if last node
        //         rangeArray[i]=range+arraySize%numSort;        //add the remainder
        //     }
        //     else{           //if not the last node
        //         rangeArray[i]=range;    //assign the equally distributed range
        //     }
        // }
    }

    int startIndex[numSort];    //array to keep the starting Index for each sorter
    for (int i=0; i<numSort; i++){
        int start=0;
        for (int j=0; j<i; j++){
            start+=rangeArray[j];
        }
        startIndex[i]=start;
    }

    // printing the rangeArray and startIndex array for testing purposes
    // int sum = 0;
    // for (int i=0; i<numSort; i++){
    //     printf("Sorter <%d>, Range: %d\n", i+1, rangeArray[i]);
    //     sum += rangeArray[i];
    // }
    // printf("SUM OF RANGE ARRAY: %d\n", sum);
    // for (int i=0; i<numSort; i++){
    //     printf("Sorter <%d>, StartINDEX: %d\n", i+1, startIndex[i]);
    // }
    /*============================================================================*/

    /*===============================SPAWN MERGER NODE================================*/
    pid_t ppid;
    ppid = getppid(); //root pid

    pid_t mpid;      //holds the process id
    if ((mpid = fork()) == 0){   //making merger node
        merger(ppid, outputFile, numSort, attributeNum, rangeArray, order, arraySize);
        exit(0);
    }
    else if (mpid == -1){
        // On error fork() returns -1.
        perror("fork failed");
        exit(EXIT_FAILURE); 
    }
    /*================================================================================*/

    //===================CREATING k SORTERS==============================//
    //https://stackoverflow.com/questions/55873277/creating-n-child-process-in-c-using-fork/55873350
    //https://www.geeksforgeeks.org/create-n-child-process-parent-process-using-fork-c/

    //array to hold all the arguments
    //0:sortingExecutable, 1:inputFile, 2:attributeNumber, 3:order, 4:sortIndex, 5:numSort, 6:arraySize, 7:range, 8:startIndex, 9:root PID, 10:NULL

    int buffSize = 100;
    char buffExecutable[buffSize];
    char buffNumSort[buffSize];
    char buffArraySize[buffSize];
    char buffRange[buffSize];
    char buffStartIndex[buffSize];
    char buffSortIndex[buffSize];
    char buffPPID[buffSize];

    strcpy(buffNumSort,to_string(numSort).c_str());
    strcpy(buffArraySize,to_string(arraySize).c_str());
    strcpy(buffPPID, to_string(ppid).c_str());

    int stat;
    pid_t pid;
    for(int i=0;i<numSort;i++) // loop will run numSort times 
    { 
        if((pid = fork()) == 0) //if it is a child
        {
            if ((i+1)%2!=0){        //if it is an odd indexed worker
                //0:sortingExecutable, 1:inputFile, 2:attributeNumber, 3:order, 4:sortIndex, 5:numSort, 6:arraySize, 7:range, 8:startIndex, 9:root PID, 10:NULL
                strcpy(buffExecutable, "./heapsort");           //use heapsort
                strcpy(buffSortIndex, to_string(i).c_str());
                strcpy(buffRange, to_string(rangeArray[i]).c_str());
                strcpy(buffStartIndex, to_string(startIndex[i]).c_str());
                char *arg[] = {buffExecutable, inputFile, attributeNum, order, buffSortIndex, buffNumSort, buffArraySize, buffRange, buffStartIndex, buffPPID, NULL};
                sort(arg);      //run the sorting function
                exit(0);        
            }
            else{           //if it is an even indexed worker
                strcpy(buffExecutable, "./bubblesort");             //use bubblesort
                strcpy(buffSortIndex, to_string(i).c_str());
                strcpy(buffRange, to_string(rangeArray[i]).c_str());
                strcpy(buffStartIndex, to_string(startIndex[i]).c_str());
                char *arg[] = {buffExecutable, inputFile, attributeNum, order, buffSortIndex, buffNumSort, buffArraySize, buffRange, buffStartIndex, buffPPID, NULL};
                sort(arg);      //run the sorting function
                exit(0); 
            }
        }
        else if (pid == -1){
            // On error fork() returns -1.
            perror("fork failed");
            exit(EXIT_FAILURE); 
        }
    }


    //================WAITING FOR THE CHILD PROCESSES TO COMPLETE==================//
    for (int i = 0; i<numSort+1; i++){          //make sure you wait for all the child processes (including the Merger node thus the +1 in the condition!)
        wait(NULL);
    }
    //===================================================================//

    int status = 42; //any non zero value
    exit(status);
}