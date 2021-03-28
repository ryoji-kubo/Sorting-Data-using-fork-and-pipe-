#include "taxpayer.h"

#ifndef HEAPSORT
#define HEAPSORT

Taxpayer** read(const char filename[], int arraySize, int range, int startIndex, int attributeNumber, int* DataArray, double* dataDoubleArray); //read the file

template <class Type> void heapifyA(Taxpayer** TaxArray, Type* dataArray, int n, int i);     //heapify function with template type for Ascending order
template <class Type> void heapSortA(Taxpayer** TaxArray, Type* dataArray, int n);  //heapSort function with template type for Ascending order

template <class Type> void heapifyD(Taxpayer** TaxArray, Type* dataArray, int n, int i);     //heapify function with template type for Descending order
template <class Type> void heapSortD(Taxpayer** TaxArray, Type* dataArray, int n);  //heapSort function with template type for Descending order


#endif