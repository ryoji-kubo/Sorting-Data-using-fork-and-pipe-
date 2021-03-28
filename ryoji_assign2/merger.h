#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "taxpayer.h"
#ifndef MERGER
#define MERGER

void merger(pid_t rootPID, char outputFile[], int numSort, char* attributeNum, int rangeArray[], char* order, int arraySize);   //main merger function
ssize_t readLine(int fd, char *buffer, size_t n);       //readLine function used to read from the fifo file
template <class Type> void merge(Taxpayer*** SortedArray, Type** dataArray, Taxpayer** FinalArray, char* order, int rangeArray[], int numSort);     //merge algo

template <class Type> void heapifyA(Type* mergeArray, int* sorterIndexArray, int n, int i);     //used for heapsorting in merge algo
template <class Type> void heapSortA(Type* mergeArray, int* sorterIndexArray, int n);

#endif