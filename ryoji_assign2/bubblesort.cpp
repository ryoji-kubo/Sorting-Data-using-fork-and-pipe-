#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<sys/time.h>
#include<csignal>
#include<fcntl.h> 
#include<sys/stat.h> 
#include<sys/types.h> 
#include<unistd.h>

#include "bubblesort.h"
#include "taxpayer.h"


int main(int argc, char *argv[]){
    //0:sortingExecutable, 1:inputFile, 2:attributeNumber, 3:order, 4:sortIndex, 5:numSort, 6:arraySize, 7:range, 8:startIndex,  9:root PID, 10:NULL


    //reading the command line
    int SIZE = 100;
    char inputFile[SIZE];
    strcpy(inputFile, argv[1]);
    int attributeNumber = atoi(argv[2]);
    char order[SIZE];
    strcpy(order, argv[3]);
    int sortIndex = atoi(argv[4]);
    int numSort = atoi(argv[5]);
    int arraySize = atoi(argv[6]);
    int range = atoi(argv[7]);
    int startIndex = atoi(argv[8]);
    pid_t rootPID = atoi(argv[9]);

    // printf("THIS IS THE %d-th BUBBLE SORTER\n",sortIndex+1);     //checks which sorter it is
    // printf("ROOT PID = %ld\n", (long) rootPID);

    //===================READING FROM THE INPUT FILE, CONSTRUCTING THE DATA ARRAYS WHICH HOLD THE SPECIFIED DATA=========================//
    Taxpayer **TaxArray;        //pointer to an array of pointers to Taxpayer obj

    int *dataArray; //pointer to an array of integers (this will hold the data of the attributes)
    dataArray = new int[range]; //dynamically allocate memory for array of integers with range of the sorter as its size
    double *dataDoubleArray;//pointer to an array of double (this will hold income)
    dataDoubleArray = new double[range];//dynamically allocate memory for array of double with range of the sorter as its size

    TaxArray = read(inputFile, arraySize, range, startIndex, attributeNumber, dataArray, dataDoubleArray);
    //================================================================================================================//

    //---------------RECORD STARTING TIME---------------//
    //To record the time it took: https://stackoverflow.com/questions/1444428/time-stamp-in-the-c-programming-language
    timeval tv1, tv2;
    gettimeofday(&tv1,NULL);
    //-------------------------------------------------//

    //==========================SORTING THE ARRAY========================================//
    //BUBBLESORT FUNCTION ADOPTED FROM https://www.geeksforgeeks.org/bubble-sort/
    if (attributeNumber==4){            //if the selected attribute is the income (use double)
        if (strcmp(order, "a")==0){
            bubbleSortA<double>(TaxArray, dataDoubleArray, range); //if ascneding, use bubblesort for ascneding
        }  
        else{
            bubbleSortD<double>(TaxArray, dataDoubleArray, range); //if descending, use descending bubblesort
        }
    }
    else {                      //if the selected attribute is other than income
        if (strcmp(order, "a")==0){
            bubbleSortA<int>(TaxArray, dataArray, range); //if ascneding, use bubblesort for ascneding
        }  
        else{
            bubbleSortD<int>(TaxArray, dataArray, range);//if ascneding, use bubblesort for descending
        }
    }
    //================================================================================================================//

    // printf("Sorted Array is....\n");
    // for (int i = 0; i<range; i++){
    //     TaxArray[i]->display();
    // }

    //================RECORD FINISHING TIME===================//
    gettimeofday(&tv2,NULL);
    long t = (tv2.tv_sec*1e6 + tv2.tv_usec) - (tv1.tv_sec*1e6 + tv1.tv_usec);
    // printf("it took %ld microseconds\n", t);
    //========================================================//


    //==================PIPING THE SORTED ARRAY TO THE MERGER NODE=========================//  
    int fd;
    char * myfifo = "myfifo";
    mkfifo(myfifo, 0777);           //make the fifo file!
    fd = open(myfifo, O_WRONLY);
    for (int i=0; i<range;i++){
        char *tmp = TaxArray[i]->write(sortIndex, t);       //write the content
        write(fd, tmp, strlen(tmp));
    }
    close(fd);
    //======================================================================================//

    //==================DELETING THE DYNAMICALLY ALLOCATED ARRAYS=====================//
    for (int i=0; i<range; i++){
        delete TaxArray[i];
    }
    delete[] TaxArray;
    delete[] dataArray;
    delete[] dataDoubleArray;
    //======================================================================================//

    //SEND SIGNAL TO ROOT
    kill(rootPID,SIGUSR1);
    // printf("%d-th BUBBLE SORTER TERMINATING\n",sortIndex+1);
    return 0;
}


Taxpayer** read(const char filename[], int arraySize, int range, int startIndex, int attributeNumber, int* dataArray, double* dataDoubleArray){     //reading the file - https://www.tutorialspoint.com/c_standard_library/c_function_fscanf.htm, https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
    int rin, zip, numDependents;
    double income;
    char first[100], last[100];             //buffer to get the content

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(filename, "r");              //open to read
    if (fp == NULL){                        //if no content
        printf("READING INPUT FILE ERROR\n");
        exit(EXIT_FAILURE);             
    }
    Taxpayer **TaxArray;        //pointer to an array of pointers
    TaxArray = new Taxpayer*[range]; //create an array of pointers to Taxpayer obj with arraySize being the number of lines in the file

    rewind(fp); //rewind the file so that we start reading from top again

    int index = 0;  //used to count which line
    int arrayIndex = 0;         //used to index in the array
    do {        //a do while to read the first line without skipping
        if (startIndex<=index && index<startIndex+range){
            fscanf(fp, "%d %s %s %d %lf %d", &rin, first, last, &numDependents, &income, &zip);     //get the content
            TaxArray[arrayIndex] = new Taxpayer(first, last, rin, zip, income, numDependents);          //construct a taxpayer object
            if (attributeNumber == 0){         //if the attribute is RIN
                dataArray[arrayIndex]=TaxArray[arrayIndex]->getRIN();        //store RIN
            }
            else if (attributeNumber == 3){        //if the attribute is number of dependents
                dataArray[arrayIndex]=TaxArray[arrayIndex]->getND();     //store # dependents
            }
            else if (attributeNumber == 4){        //if the attribute is income
                dataDoubleArray[arrayIndex]=TaxArray[arrayIndex]->getIncome();       //store income (in the double!!)
            }
            else if (attributeNumber == 5){        //if the attribute is zip
                dataArray[arrayIndex]=TaxArray[arrayIndex]->getZIP();        //store zip
            }
            // if (index==startIndex || index==startIndex+range-1){     //print function for testing purposes
            //     printf("<%d>",arrayIndex+startIndex);
            //     TaxArray[arrayIndex]->display();
            //     if (attributeNumber==4){
            //         printf("Data: %.2lf\n",dataDoubleArray[arrayIndex]);
            //     }
            //     else{
            //         printf("Data: %d\n",dataArray[arrayIndex]);
            //     }
            // } 
            arrayIndex++;
        }
        index++;
    }while((read = getline(&line, &len, fp)) != -1 && index != arraySize);  //second condition to not double read last line
    fclose(fp);
    if (line)
        free(line);             //free the memory used for line

    return TaxArray;
}

template <class Type>
void bubbleSortA(Taxpayer** TaxArray, Type* dataArray, int n)  
{  
    int i, j;  
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++){
            if (dataArray[j] > dataArray[j+1]){
                Type tmp = dataArray[j];
                dataArray[j] = dataArray[j+1];
                dataArray[j+1] = tmp;

                Taxpayer* Tmp = TaxArray[j];
                TaxArray[j] = TaxArray[j+1];
                TaxArray[j+1] = Tmp;
            }
        }  
    }      
}  

template <class Type>
void bubbleSortD(Taxpayer** TaxArray, Type* dataArray, int n)  
{  
    int i, j;  
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++){
            if (dataArray[j] < dataArray[j+1]){
                Type tmp = dataArray[j];
                dataArray[j] = dataArray[j+1];
                dataArray[j+1] = tmp;

                Taxpayer* Tmp = TaxArray[j];
                TaxArray[j] = TaxArray[j+1];
                TaxArray[j+1] = Tmp;
            }
        }  
    }      
}  