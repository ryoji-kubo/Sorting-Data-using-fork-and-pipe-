#include <cstdio>
#include <cstring>
#include <string>
#include <fcntl.h> 
#include <errno.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <csignal>

// #include <iostream>
// #include <fstream>

#include "merger.h"
#include "taxpayer.h"

using namespace std;

void merger(pid_t rootPID, char outputFile[], int numSort, char* attributeNum, int rangeArray[], char* order, int arraySize){

    //===========CREATING THE NECESSARY ARRAYS===============//
    Taxpayer ***SortedArray; //Each element will hold a Sorted array from each sorter (This will be a 2D array)
    SortedArray = new Taxpayer**[numSort]; //create an array for each sorter
    for (int i=0; i<numSort; i++){
        SortedArray[i] = new Taxpayer*[rangeArray[i]];  //for each array, it will hold Taxpayer obj corresponding to the specific range
    }

    int **dataArray; //each element will hold a sorted data array from each sorter (2D array)
    dataArray = new int*[numSort];
    for (int i=0; i<numSort; i++){
        dataArray[i] = new int[rangeArray[i]];
    }
    
    double **dataDoubleArray; //each element will hold a sorted data array from each sorter (2D array) THIS IS FOR INCOME SPECIFIC
    dataDoubleArray = new double*[numSort];
    for (int i=0; i<numSort; i++){
        dataDoubleArray[i] = new double[rangeArray[i]];
    }
    //======================================================//

    //=================READING THE FIFO FILE AND INSERTING DATA INTO THE ARRAY===================//
    //the necessary variables
    int attribute = atoi(attributeNum);
    int size = 1000;
    char str1[size];        //the str buffer to read the line
    int index = 0;          //used to see how many we have read
    char first[100];
    char last[100];
    int rin, zip, numD, sortID;
    double income;

    long timeArray[numSort]; //this array will hold the time stats for each sorter
    long time;

    ssize_t numRead;    //this will be used to see how many char we have read
    
    int sortPosArr[numSort];        //this array will keep count of how many we data we have read from each sorter
    for (int i=0; i<numSort; i++){
        sortPosArr[i] = 0;      //initialize with 0
    }

    int fd1; 
    // FIFO file path
    char * myfifo = "myfifo";
    mkfifo(myfifo, 0777);   //make the fifo file
    fd1 = open(myfifo,O_RDONLY);  //open the fifo file

    while(index<arraySize){         //while we haven't read all of the data
        numRead = readLine(fd1, str1, 1000);    //read until the \n delimiter!
        
        if (numRead == 0){          //if nothing read
            continue;       //continue reading without incrementing the index
        }

        sscanf(str1, "%d %s %s %d %d %lf %d %ld", &sortID, first, last, &rin, &zip, &income, &numD, &time); //scan the string
        SortedArray[sortID][sortPosArr[sortID]] = new Taxpayer(first, last, rin, zip, income, numD);    //create taxpayer obj at the specifc index
        if (attribute == 0){         //if the attribute is RIN
            dataArray[sortID][sortPosArr[sortID]]=SortedArray[sortID][sortPosArr[sortID]]->getRIN();        //store RIN
        }
        else if (attribute == 3){        //if the attribute is number of dependents
            dataArray[sortID][sortPosArr[sortID]]=SortedArray[sortID][sortPosArr[sortID]]->getND();        //store numD
        }
        else if (attribute == 4){        //if the attribute is income
            dataDoubleArray[sortID][sortPosArr[sortID]]=SortedArray[sortID][sortPosArr[sortID]]->getIncome();        //store Income
        }
        else if (attribute == 5){        //if the attribute is zip
            dataArray[sortID][sortPosArr[sortID]]=SortedArray[sortID][sortPosArr[sortID]]->getND();        //store zip
        }
        timeArray[sortID] = time;   //store time
        index++;    //increment index (out of total data)
        sortPosArr[sortID]++;       //increment the position for that specific sorter
    }
    close(fd1); 

    //============================================================//
    

    //Print testing to see if it has been read correctly
    // int start = 0;
    // for (int i=0; i<numSort; i++){
    //     for (int j=0; j<rangeArray[i]; j++){
    //         if ((start+j)%10 == 0){
    //             if (attribute == 4){
    //                 printf("<%d> %.2lf ", start+j, dataDoubleArray[i][j]);
    //                 SortedArray[i][j]->display();
    //             }
    //             else{
    //                 printf("<%d> %d ", start+j, dataArray[i][j]); 
    //                 SortedArray[i][j]->display();
    //             }
    //         }
    //     }
    //     start += rangeArray[i];
    //     printf("=================\n");
    // }

    //======================MERGE SORTING============================//
    Taxpayer **FinalArray;        //This will be the final output array
    FinalArray = new Taxpayer*[arraySize];  

    timeval tv1, tv2; //record starting time
    gettimeofday(&tv1,NULL);

    if (attribute == 4){ //if sorting income
        merge<double>(SortedArray, dataDoubleArray, FinalArray, order, rangeArray, numSort);
    }
    else{
        merge<int>(SortedArray, dataArray, FinalArray, order, rangeArray, numSort);
    }

    gettimeofday(&tv2,NULL);//record finishing time
    long t = (tv2.tv_sec*1e6 + tv2.tv_usec) - (tv1.tv_sec*1e6 + tv1.tv_usec);
    //==============================================================//

    //PRINT TESTING TO SEE IF MERGE SORTING WORKED
    for (int i=0; i<arraySize; i++){
        if (i%1000 == 0){
            printf("<%d>: ", i);
            if (attribute == 0){         //if the attribute is RIN
                printf("%d   ", FinalArray[i]->getRIN());        //store RIN
                FinalArray[i]->display();
            }
            else if (attribute == 3){        //if the attribute is number of dependents
                printf("%d   ", FinalArray[i]->getND());     //store # dependents
                FinalArray[i]->display();
            }
            else if (attribute == 4){        //if the attribute is income
                printf("%.2lf   ", FinalArray[i]->getIncome());       //store income (in the double!!)
                FinalArray[i]->display();
            }
            else if (attribute == 5){        //if the attribute is zip
                printf("%d   ", FinalArray[i]->getZIP());        //store zip
                FinalArray[i]->display();
            }
        }
    }


    //=============STDOUT PRINTING================//

    // printf("+====================OUTPUT====================+\n");
    // for (int i=0; i<arraySize; i++){
    //     if (i%1 == 0){
    //         printf("<%d>: ", i+1);
    //         FinalArray[i]->display();
    //     }
    // }
    // printf("+==============================================+\n");

    printf("+======Time Statistics (Odd # - HeapSort, Even # - BubbleSort)======+\n");
    for (int i = 0; i<numSort; i++){
        printf("Sorter<%d> %ld microseconds\n", i+1, timeArray[i]);
    }
    printf("Merging Algo took %ld microseconds\n", t);
    printf("+===================================================================+\n");

    //============================================//

    //=============WRITING TO OUTPUT FILE=============//
    FILE *fp;
    fp = fopen(outputFile, "w");
    for (int i = 0; i<arraySize; i++){
        fprintf(fp, "%s", FinalArray[i]->output());
    }
    fprintf(fp, "\n======Time Statistics (Odd # - HeapSort, Even # - BubbleSort)======\n");
    for (int i = 0; i<numSort; i++){
        fprintf(fp, "Sorter<%d> %ld microseconds\n", i+1, timeArray[i]);
    }
    fprintf(fp, "Merging Algo took %ld microseconds", t);
    fclose(fp);


    //ALTERNATIVE WRITE METHOD USING FSTERAM AND IOSTREAM
    // ofstream outFile;
    // outFile.open(outputFile, ios::out);
    // for (int i = 0; i<arraySize; i++){
    //     outFile << FinalArray[i]->output();
    // }
    // outFile<<"\n======Time Statistics (Odd # - HeapSort, Even # - BubbleSort)======\n";
    // for (int i = 0; i<numSort; i++){
    //     outFile<< "Sorter<"<<i+1<<"> "<<timeArray[i]<<" microseconds\n";
    // }
    // outFile << "Merging Algo took "<<t<<" microseconds\n";
    // outFile.close();
    //=========================================//

    //SEND SIGNAL TO ROOT
    kill(rootPID,SIGUSR2);


    //===================DISALLOCATING MEMORY============//
    for (int i=0; i<numSort; i++){
        for (int j=0; j<rangeArray[i];j++){
            delete SortedArray[i][j];
        }
        delete[] SortedArray[i];
    }
    delete[] SortedArray;

    for (int i=0; i<numSort; i++){
        delete[] dataArray[i];
    }
    delete[] dataArray;

    for (int i=0; i<numSort; i++){
        delete[] dataDoubleArray[i];
    }
    delete[] dataDoubleArray;
    
    delete[] FinalArray;
    //=================================================//
}

ssize_t readLine(int fd, char *buffer, size_t n)  //readLine function adopted from M. Kerrisk. The Linux Programming Interface: A Linux and UNIX System Programming. Page 1201
{
    ssize_t numRead;
    size_t totRead;
    char *buf;
    char ch;
    if (n <= 0 || buffer == NULL) {
        errno = EINVAL; return -1; 
    }
    buf = buffer;
    totRead = 0;
    for (;;) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        else if (numRead == 0) {
            if (totRead == 0)
                return 0;
            else
                break;
        }
        else {
            if (totRead < n - 1) {
                totRead++;
                *buf++ = ch; 
            }
            if (ch == '\n')
                break;
        }
    }
    *buf = '\0';
    return totRead;
}



template <class Type>
void merge(Taxpayer*** SortedArray, Type** dataArray, Taxpayer** FinalArray, char* order, int rangeArray[], int numSort){
    bool ascend = true;
    if (strcmp(order, "a")!=0){ //if it is descending order
        ascend = false;         //flag it as false
    }

    Type dummy; //use this variable as a filler once you complete sorting for a sortedArray
    if (ascend){ //if it is ascending order
        dummy = 99999999;             //the dummy variable should be very large
    }
    else{   //if it is descending order
        dummy = -1;             //dummy is a negative number
    }

    int indexArray[numSort];        //indexArray records at which index of the sorted array from each sorter we are at.
    Type mergeArray[numSort];   //mergeArray will be the main array used to sort the merged elements from each sorted array.
    int sorterIndexArray[numSort]; //elements of sorterIndexArray corresponds with the index of the sorted array for each element in mergeArray

    for (int i=0; i<numSort; i++){  //INITIALIZATION
        indexArray[i] = 0; //initialize index of each sorted array to be 0.
        sorterIndexArray[i] = i; //at first, the elements of mergeArray come in an ascending order of sorter array (1, 2, 3, 4, ...)

        if (rangeArray[i]!=0){  //if the sortedArray for that sorter is not empty
            mergeArray[i] = dataArray[i][0]; //at first, the mergeArray receives the first element from each sorted array
        }
        else{ //if there is no element to extract
            mergeArray[i] = dummy;
        }
    }
    heapSortA(mergeArray, sorterIndexArray, numSort); //sort the mergeArray and reflect the same change to the sorterIndexArray in ascending way


    int index = 0;      //this variable holds the index out of the total data points
    int SorterCount = 0; //this variable is used to see how many of the sortedArrays we have finished checking.
    int sorterIndex; //this holds the sorterIndex of the max/min value in the mergeArray
    int position; //this holds the position from each sortedArray

    while(1){   //the loop to merge the array
        if (ascend){
            sorterIndex = sorterIndexArray[0];      //if ascending, get the min value which is at index 0 of the mergeArray (and sorterIndexArray has the same order!)
        }
        else{
            sorterIndex = sorterIndexArray[numSort-1];      //if descending, get the max value which is at the last index of the mergeArray (and sorterIndexArray has the same order!)
        }
        position = indexArray[sorterIndex]; //get the position in the sortedArray

        FinalArray[index] = SortedArray[sorterIndex][position]; //get the corresponding obj and copy to the FinalArray

        if (ascend){
            if (position+1<rangeArray[sorterIndex]){ //if we still have not checked all the elements from the same sortedArray
                mergeArray[0] = dataArray[sorterIndex][position+1];     //replace the first element of the mergeArray with the next element from the same sortedArray
                indexArray[sorterIndex]++;      //increment the index of the same sortedArray
            }
            else{           //if we have checked all the elements from the same sortedArray
                mergeArray[0] = dummy;      //fill the mergeArray by a dummy variable
                SorterCount++; //increment SorterCount
                if (SorterCount == numSort){    //if we have looked through the work of all the sorters
                    break;      //break the loop
                }
            }
        }
        else{
            if (position+1<rangeArray[sorterIndex]){ //if we still have not checked all the elements from the same sortedArray
                mergeArray[numSort-1] = dataArray[sorterIndex][position+1];     //replace the last element of the mergeArray with the next element from the same sortedArray
                indexArray[sorterIndex]++;      //increment the index of the same sortedArray
            }
            else{           //if we have checked all the elements from the same sortedArray
                mergeArray[numSort-1] = dummy;      //fill the mergeArray by a dummy variable
                SorterCount++; //increment SorterCount
                if (SorterCount == numSort){    //if we have looked through the work of all the sorters
                    break;  //break the loop
                }
            }
        }
        heapSortA<Type>(mergeArray, sorterIndexArray, numSort);     //heapsort the mergeArray and reflect the same changes to sorterIndexArray
        index++; //increment index in the final array
    }
}

// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
template <class Type>
void heapifyA(Type* mergeArray, int* sorterIndexArray, int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && mergeArray[l] > mergeArray[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && mergeArray[r] > mergeArray[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        Type tmp = mergeArray[i];
        mergeArray[i] = mergeArray[largest];
        mergeArray[largest] = tmp;

        int Tmp = sorterIndexArray[i];
        sorterIndexArray[i] = sorterIndexArray[largest];
        sorterIndexArray[largest] = Tmp;


        // Recursively heapify the affected sub-tree
        heapifyA<Type>(mergeArray, sorterIndexArray, n, largest);
    }
}

//function to do heap sort
template <class Type>
void heapSortA(Type* mergeArray, int* sorterIndexArray, int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapifyA<Type>(mergeArray, sorterIndexArray, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        Type tmp = mergeArray[0];
        mergeArray[0] = mergeArray[i];
        mergeArray[i] = tmp;

        int Tmp = sorterIndexArray[0];
        sorterIndexArray[0] = sorterIndexArray[i];
        sorterIndexArray[i] = Tmp;

        // call max heapify on the reduced heap
        heapifyA<Type>(mergeArray, sorterIndexArray, i, 0);
    }
}