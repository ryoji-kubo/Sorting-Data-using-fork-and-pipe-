#include <cstdio>
#include<cstdlib>
#include <cstring>
#include <unistd.h>

#include "taxpayer.h"
#include "sort.h"

void sort(char **arg){
    
    //0:sortingExecutable, 1:inputFile, 2:attributeNumber, 3:order, 4:sortIndex, 5:numSort, 6:arraySize, 7:range, 8:startIndex, 9:root PID, 10:NULL

    //===========================THE EXEC() CALL TO EXECUTE SORTING==============================//
    //Citation: https://pubs.opengroup.org/onlinepubs/9699919799/functions/getcwd.html
    //Setting the path
    long path_max;
    size_t size;

    path_max = pathconf(".", _PC_PATH_MAX);
    if (path_max == -1)
        size = 1024;
    else if (path_max > 10240)
        size = 10240;
    else
        size = path_max;

    char cwd[path_max];
    getcwd(cwd, sizeof(cwd));

    if ((atoi(arg[4])+1)%2!=0){ //if it is an odd indexed worker
        strcat(cwd, "/heapsort");       //choose the heapsort file
    }
    else{                       //if even
        strcat(cwd, "/bubblesort");      //choose the bubblesort file
    }   
    execv(cwd, arg);        //execute the corresponding sorting executable!
    //============================================================================================//
}

