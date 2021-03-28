#include "taxpayer.h"

#ifndef BUBBLESORT
#define BUBBLESORT

Taxpayer** read(const char filename[], int arraySize, int range, int startIndex, int attributeNumber, int* DataArray, double* dataDoubleArray); //read the file
template <class Type> void bubbleSortA(Taxpayer** TaxArray, Type* dataArray, int n);  //bubbleSort function with template type for Ascending order
template <class Type> void bubbleSortD(Taxpayer** TaxArray, Type* dataArray, int n);  //bubbleSort function with template type for Descending order


#endif