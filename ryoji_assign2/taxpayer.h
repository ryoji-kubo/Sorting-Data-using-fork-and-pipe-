#ifndef TAXPAYER
#define TAXPAYER

const int size = 100;  //declaring the max length of first and last name as 100.
//using namespace std;

class Taxpayer
{
private:
    char first[size];   //first name
    char last[size];    //last name
    int RIN;        //Residential Identification Number
    int zip;        //postal-code
    double income;     //income
    int numDependents;  //number of dependents
    char* arr; //an array of characters used for writing (fifo)
    char* arr2; //an array of characters used for the output file writing      
public:
    Taxpayer(const char f[], const char l[], int rin, int z, double i, int numD); //constructor -- reason for the const char* - https://stackoverflow.com/questions/1524356/c-deprecated-conversion-from-string-constant-to-char
    ~Taxpayer(); //destructor to delete the char arr which has dynamically allocated memory
    const char* getFirst();
    const char* getLast();
    int getRIN();   //gets the RIN
    int getZIP();   //gets the ZIP
    double getIncome(); //gets the income
    int getND(); //gets the number of dependents
    void display(); //displays the content of the Taxpayer in a line    
    char* write(int sorterID, long time); //writes a character string with the sorterID and time
    char* output(); //used for making the output file
};

#endif