#include<cstdio>
#include<cstring>
#include<string>
#include "taxpayer.h"

using namespace std;

Taxpayer::Taxpayer(const char f[], const char l[], int rin, int z, double i, int numD){
    strcpy(first, f);
    strcpy(last, l);
    RIN = rin;
    zip = z;
    income = i;
    numDependents = numD;
    arr = new char[500]; //dynamically allocate memory for the character array
    arr2 = new char[500];
}
Taxpayer::~Taxpayer(){
    delete[] arr; //make sure you delete the char array!
    delete[] arr2;
}
const char* Taxpayer::getFirst(){
    return first;
}
const char* Taxpayer::getLast(){
    return last;
}
int Taxpayer::getRIN(){
    return RIN;
}

int Taxpayer::getZIP(){
    return zip;
}

double Taxpayer::getIncome(){
    return income;
}

int Taxpayer::getND(){
    return numDependents;
}

void Taxpayer::display(){
    printf("Name: %s %s RIN: %d ZIP: %d Income: %.2lf Number of Dependants: %d\n" , first, last, RIN, zip, income, numDependents);   
}

char* Taxpayer::write(int sorterID, long time){
    char tmp[255];
    sprintf(tmp, "%ld", time);  //converts long to char*

    strcat(arr, const_cast<char*> (to_string(sorterID).c_str()));
    strcat(arr, " ");
    strcat(arr, first);
    strcat(arr, " ");
    strcat(arr, last);
    strcat(arr, " ");
    strcat(arr, const_cast<char*> (to_string(RIN).c_str()));
    strcat(arr, " ");
    strcat(arr, const_cast<char*> (to_string(zip).c_str()));
    strcat(arr, " ");
    strcat(arr, const_cast<char*> (to_string(income).c_str()));
    strcat(arr, " ");
    strcat(arr, const_cast<char*> (to_string(numDependents).c_str()));
    strcat(arr, " ");
    strcat(arr, tmp);
    strcat(arr, "\n");      //add delimiter!
    return arr;
}

char* Taxpayer::output(){
    strcat(arr2, const_cast<char*> (to_string(RIN).c_str()));
    strcat(arr2, " ");
    strcat(arr2, first);
    strcat(arr2, " ");
    strcat(arr2, last);
    strcat(arr2, " ");
    strcat(arr2, const_cast<char*> (to_string(numDependents).c_str()));
    strcat(arr2, " ");
    strcat(arr2, const_cast<char*> (to_string(income).c_str()));
    strcat(arr2, " ");
    strcat(arr2, const_cast<char*> (to_string(zip).c_str()));
    strcat(arr2, "\n");
    return arr2;
}