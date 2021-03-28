#include "taxpayer.h"
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include <unistd.h>   // getpid(), getppid(),fork() exec()
#include <sys/wait.h> // wait()

void child() {
  int status = 42; //any non zero value

  printf(" CHILD <%ld> with PPID = %ld is sleeping. Wait....\n",
         (long) getpid(), (long) getppid());

  sleep(3);

  printf(" CHILD <%ld> I am awake now, Goodbye! Exit with status %d.\n",
         (long) getpid(), status);
 
  exit(status);
}

void parent(pid_t pid) {
  int status;

  printf("PARENT <%ld> Spawned a child with PID = %ld.\n",
         (long) getpid(), (long) pid);

  //wait for a specific child process with specific pid
  waitpid(pid , &status , 0);

  printf("PARENT <%ld> had a terminated child which had a PID = %ld.\n",
    (long) getpid(), (long) pid);


  if (WIFEXITED(status)) {
    printf("PARENT <%ld> Child with PID = %ld and exit status = %d terminated.\n",
           (long) getpid(), (long) pid, WEXITSTATUS(status));
  }

  printf("PARENT <%ld> Goodbye!\n",
         (long) getpid());

  exit(EXIT_SUCCESS);
}

int main(){
       Taxpayer **TaxArray;
       int arraySize = 5;
       TaxArray = new Taxpayer*[arraySize]; //create an array of pointers to Taxpayer obj with arraySize being the number of lines in the file
       
       // for (int i=0; i<arraySize;i++){
       //     char input[15000];
       //     char i1[100], i2[100];
       //     int i3, i4, i5, i6;           //buffer for each argument
       //     printf("Please input First, Last, RIN, ZIP, Income, #Dependents\n");
       //     fgets(input, 15000, stdin);     //get the input from prompt
       //     int option = sscanf(input, "%s %s %d %d %d %d", i1, i2, &i3, &i4, &i5, &i6);       //scan and see how many input you got (matches)
              
       //     TaxArray[i] = new Taxpayer(i1, i2, i3, i4, i5, i6);          //construct a taxpayer object
       // }

       TaxArray[0] = new Taxpayer("Ryoji", "Kubo", 923, 3050047, 1000000, 0);
       TaxArray[1] = new Taxpayer("Aayusha", "Shresha", 1234, 88888, 9000, 0);
       TaxArray[2] = new Taxpayer("Mizuho", "Kubo", 5678, 405098, 100000, 3);
       TaxArray[3] = new Taxpayer("Kota", "Kubo", 333, 986111, 100500, 1);
       TaxArray[4] = new Taxpayer("Hiroshi", "Kubo", 2345, 900099, 50000, 5);

       // for (int i=0; i<arraySize; i++){
       //        TaxArray[i]->display();
       // }
       // long path_max;
       // size_t size;

       // path_max = pathconf(".", _PC_PATH_MAX);
       // if (path_max == -1)
       //        size = 1024;
       // else if (path_max > 10240)
       //        size = 10240;
       // else
       //        size = path_max;

       // char cwd[path_max];
       // char *arg[] = {"./heapsort",NULL}; 
       // getcwd(cwd, sizeof(cwd));
       // strcat(cwd, "/heapsort");
      
       // execv(cwd, arg);

       pid_t pid;
       switch (pid = fork()) {
       case -1:
           // On error fork() returns -1.
           perror("fork failed");
           exit(EXIT_FAILURE);
       case 0:
           // On success fork() returns 0 in the child.
           child();
       default:
           // On success fork() returns the pid of the child to the parent.
           parent(pid);
       }
       Taxpayer **SortArray;   //pointer to an array of pointer to Taxpayer object
       SortArray = new Taxpayer*[2];    //create an array of pointers to Taxpayer object (dynamically allocated) the size is determined by the rangeArray
       for (int i=0; i<2; i++){
           if (i == 0){
               SortArray[i] = TaxArray[i];
           }
           else
       }



       for (int i=0; i<arraySize; i++){
              delete TaxArray[i];
       }
       delete[] TaxArray;
       return 0;
}