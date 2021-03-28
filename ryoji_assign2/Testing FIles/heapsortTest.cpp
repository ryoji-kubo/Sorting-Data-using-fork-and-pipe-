#include "taxpayer.h"
#include <cstring>
#include <cstdio>
// template <class Type>
// void heapifyA(Taxpayer** TaxArray, Type* dataArray, int n, int i);

// template <class Type>
// void heapSortA(Taxpayer** TaxArray, Type* dataArray, int n);

// template <class Type>
// void heapifyD(Taxpayer** TaxArray, Type* dataArray, int n, int i);

// template <class Type>
// void heapSortD(Taxpayer** TaxArray, Type* dataArray, int n);





int main(){
    int dataArray[] = {12, 6, 5, 9, 0, 3};
    Taxpayer** TaxArray;
    TaxArray = new Taxpayer*[0];

    int* dataArray2 = new int[0];
    // TaxArray[0] = new Taxpayer("Ryoji", "Kubo", 923, 3050047, 1000000, 12);
    // TaxArray[1] = new Taxpayer("Aayusha", "Shresha", 1234, 88888, 9000, 6);
    // TaxArray[2] = new Taxpayer("Mizuho", "Kubo", 5678, 405098, 100000, 5);
    // TaxArray[3] = new Taxpayer("Kota", "Kubo", 333, 986111, 100500, 9);
    // TaxArray[4] = new Taxpayer("Hiroshi", "Kubo", 2345, 900099, 50000, 0);
    // TaxArray[5] = new Taxpayer("A", "B", 2345, 900099, 50000, 3);
    // heapSortD<int>(TaxArray, dataArray, 6);

    // printf("dataArray: ");
    // for (int i=0; i<6; i++){
    //     printf("%d ", dataArray[i]);
    // }
    // printf("\n");
    // printf("TaxArray: \n");
    // for (int i=0; i<6; i++){
    //     TaxArray[i]->display();
    // }

    for (int i=0; i<0; i++){
        delete TaxArray[i];
    }
    delete[] TaxArray;

    delete[] dataArray2;




}

// template <class Type>
// void heapifyA(Taxpayer** TaxArray, Type* dataArray, int n, int i)
// {
//     int largest = i; // Initialize largest as root
//     int l = 2 * i + 1; // left = 2*i + 1
//     int r = 2 * i + 2; // right = 2*i + 2

//     // If left child is larger than root
//     if (l < n && dataArray[l] > dataArray[largest])
//         largest = l;

//     // If right child is larger than largest so far
//     if (r < n && dataArray[r] > dataArray[largest])
//         largest = r;

//     // If largest is not root
//     if (largest != i) {
//         Type tmp = dataArray[i];
//         dataArray[i] = dataArray[largest];
//         dataArray[largest] = tmp;

//         Taxpayer* Tmp = TaxArray[i];
//         TaxArray[i] = TaxArray[largest];
//         TaxArray[largest] = Tmp;


//         // Recursively heapify the affected sub-tree
//         heapifyA<Type>(TaxArray, dataArray, n, largest);
//     }
// }

// //function to do heap sort
// template <class Type>
// void heapSortA(Taxpayer** TaxArray, Type* dataArray, int n)
// {
//     // Build heap (rearrange array)
//     for (int i = n / 2 - 1; i >= 0; i--)
//         heapifyA<Type>(TaxArray, dataArray, n, i);

//     // One by one extract an element from heap
//     for (int i = n - 1; i > 0; i--) {
//         // Move current root to end
//         Type tmp = dataArray[0];
//         dataArray[0] = dataArray[i];
//         dataArray[i] = tmp;

//         Taxpayer* Tmp = TaxArray[0];
//         TaxArray[0] = TaxArray[i];
//         TaxArray[i] = Tmp;

//         // call max heapify on the reduced heap
//         heapifyA<Type>(TaxArray, dataArray, i, 0);
//     }
// }

// template <class Type>
// void heapifyD(Taxpayer** TaxArray, Type* dataArray, int n, int i)
// {
//     int smallest = i; // Initialize smallest as root
//     int l = 2 * i + 1; // left = 2*i + 1
//     int r = 2 * i + 2; // right = 2*i + 2

//     // If left child is smaller than root
//     if (l < n && dataArray[l] < dataArray[smallest])
//         smallest = l;

//     // If right child is smaller than smallest so far
//     if (r < n && dataArray[r] < dataArray[smallest])
//         smallest = r;

//     // If smallest is not root
//     if (smallest != i) {
//         Type tmp = dataArray[i];
//         dataArray[i] = dataArray[smallest];
//         dataArray[smallest] = tmp;

//         Taxpayer* Tmp = TaxArray[i];
//         TaxArray[i] = TaxArray[smallest];
//         TaxArray[smallest] = Tmp;


//         // Recursively heapify the affected sub-tree
//         heapifyD<Type>(TaxArray, dataArray, n, smallest);
//     }
// }

// //function to do heap sort
// template <class Type>
// void heapSortD(Taxpayer** TaxArray, Type* dataArray, int n)
// {
//     // Build heap (rearrange array)
//     for (int i = n / 2 - 1; i >= 0; i--)
//         heapifyD(TaxArray, dataArray, n, i);

//     // One by one extract an element from heap
//     for (int i = n - 1; i > 0; i--) {
//         // Move current root to end
//         Type tmp = dataArray[0];
//         dataArray[0] = dataArray[i];
//         dataArray[i] = tmp;

//         Taxpayer* Tmp = TaxArray[0];
//         TaxArray[0] = TaxArray[i];
//         TaxArray[i] = Tmp;

//         // call max heapify on the reduced heap
//         heapifyD<Type>(TaxArray, dataArray, i, 0);
//     }
// }