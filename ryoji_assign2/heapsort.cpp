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

#include "heapsort.h"
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

    // printf("THIS IS THE %d-th HEAP SORTER\n",sortIndex+1);     //checks which sorter it is

    //===================READING FROM THE INPUT FILE, CONSTRUCTING THE DATA ARRAYS WHICH HOLD THE SPECIFIED DATA=========================//
    Taxpayer **TaxArray;        //pointer to an array of pointers (this will hold an array of pointers of object taxpayer)

    int *dataArray; //pointer to an array of integers (this will hold the data of the attributes)
    dataArray = new int[range]; //dynamically allocate memory for array of integers with range of the sorter as its size
    double *dataDoubleArray;//pointer to an array of double (this will hold income)
    dataDoubleArray = new double[range];//dynamically allocate memory for array of double with range of the sorter as its size

    TaxArray = read(inputFile, arraySize, range, startIndex, attributeNumber, dataArray, dataDoubleArray);
    //================================================================================================================//

    //===================RECORD THE STARTING TIME============================//
    //To record the time it took: https://stackoverflow.com/questions/1444428/time-stamp-in-the-c-programming-language
    timeval tv1, tv2;
    gettimeofday(&tv1,NULL);
    //========================================================================//

    //==========================SORTING THE ARRAY========================================//
    //HEAPSORT FUNCTION ADOPTED FROM https://www.geeksforgeeks.org/write-a-program-to-reverse-an-array-or-string/
    if (attributeNumber==4){            //if the selected attribute is the income (use double)
        if (strcmp(order, "a")==0){     //if order is ascending
            heapSortA<double>(TaxArray, dataDoubleArray, range);    //use the ascending heapsort function
        }  
        else{
            heapSortD<double>(TaxArray, dataDoubleArray, range);        //else use the descending heapsort function
        }
    }
    else {                      //if the selected attribute is other than income
        if (strcmp(order, "a")==0){
            heapSortA<int>(TaxArray, dataArray, range);     //use ascending heapsort function
        }  
        else{
            heapSortD<int>(TaxArray, dataArray, range);         //use descending heapsort function 
        }
    }
    //========================================================================//

    //===================RECORD THE FINISHING TIME============================//
    gettimeofday(&tv2,NULL);
    long t = (tv2.tv_sec*1e6 + tv2.tv_usec) - (tv1.tv_sec*1e6 + tv1.tv_usec);
    // printf("it took %ld microseconds\n", t);
    //========================================================================//

    // printf("Sorted Array is....\n");
    // for (int i = 0; i<range; i++){
    //     TaxArray[i]->display();
    // }

    //==================PIPING THE SORTED ARRAY TO THE MERGER NODE=========================//
    int fd;
    char * myfifo = "myfifo";
    mkfifo(myfifo, 0777);       //make the fifo file!
    fd = open(myfifo, O_WRONLY);    //open the fifo
    for (int i=0; i<range;i++){
        char *tmp = TaxArray[i]->write(sortIndex, t);       //write the sorted array content
        write(fd, tmp, strlen(tmp));
    }
    close(fd);      //close!
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
    // printf("%d-th HEAP SORTER TERMINATING\n",sortIndex+1);
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
            arrayIndex++;
        }
        index++;
    }while((read = getline(&line, &len, fp)) != -1 && index != arraySize);  //second condition to not double read last line
    fclose(fp);
    if (line)
        free(line);             //free the memory used for line

    return TaxArray;
}

// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
template <class Type>
void heapifyA(Taxpayer** TaxArray, Type* dataArray, int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && dataArray[l] > dataArray[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && dataArray[r] > dataArray[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        Type tmp = dataArray[i];
        dataArray[i] = dataArray[largest];
        dataArray[largest] = tmp;

        Taxpayer* Tmp = TaxArray[i];
        TaxArray[i] = TaxArray[largest];
        TaxArray[largest] = Tmp;


        // Recursively heapify the affected sub-tree
        heapifyA<Type>(TaxArray, dataArray, n, largest);
    }
}

//function to do heap sort
template <class Type>
void heapSortA(Taxpayer** TaxArray, Type* dataArray, int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapifyA<Type>(TaxArray, dataArray, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        Type tmp = dataArray[0];
        dataArray[0] = dataArray[i];
        dataArray[i] = tmp;

        Taxpayer* Tmp = TaxArray[0];
        TaxArray[0] = TaxArray[i];
        TaxArray[i] = Tmp;

        // call max heapify on the reduced heap
        heapifyA<Type>(TaxArray, dataArray, i, 0);
    }
}


// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
template <class Type>
void heapifyD(Taxpayer** TaxArray, Type* dataArray, int n, int i)
{
    int smallest = i; // Initialize smallest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is smaller than root
    if (l < n && dataArray[l] < dataArray[smallest])
        smallest = l;

    // If right child is smaller than smallest so far
    if (r < n && dataArray[r] < dataArray[smallest])
        smallest = r;

    // If smallest is not root
    if (smallest != i) {
        Type tmp = dataArray[i];
        dataArray[i] = dataArray[smallest];
        dataArray[smallest] = tmp;

        Taxpayer* Tmp = TaxArray[i];
        TaxArray[i] = TaxArray[smallest];
        TaxArray[smallest] = Tmp;


        // Recursively heapify the affected sub-tree
        heapifyD<Type>(TaxArray, dataArray, n, smallest);
    }
}

//function to do heap sort
template <class Type>
void heapSortD(Taxpayer** TaxArray, Type* dataArray, int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapifyD(TaxArray, dataArray, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        Type tmp = dataArray[0];
        dataArray[0] = dataArray[i];
        dataArray[i] = tmp;

        Taxpayer* Tmp = TaxArray[0];
        TaxArray[0] = TaxArray[i];
        TaxArray[i] = Tmp;

        // call max heapify on the reduced heap
        heapifyD<Type>(TaxArray, dataArray, i, 0);
    }
}