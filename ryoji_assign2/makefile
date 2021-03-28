CC = g++

all: myhie heapsort bubblesort

myhie: myhie.o coord.o merger.o sort.o taxpayer.o
	$(CC) -o myhie myhie.o coord.o merger.o sort.o taxpayer.o

myhie.o: myhie.cpp coord.h taxpayer.h
	$(CC) -c myhie.cpp

coord.o: coord.cpp merger.h sort.h taxpayer.h
	$(CC) -c coord.cpp

sort.o: sort.cpp taxpayer.h
	$(CC) -c sort.cpp

merger.o: merger.cpp taxpayer.h
	$(CC) -c merger.cpp

taxpayer.o: taxpayer.cpp
	$(CC) -c taxpayer.cpp

heapsort: heapsort.o taxpayer.o
	$(CC) -o heapsort heapsort.o taxpayer.o

heapsort.o: heapsort.cpp taxpayer.h
	$(CC) -c heapsort.cpp

bubblesort: bubblesort.o taxpayer.o
	$(CC) -o bubblesort bubblesort.o taxpayer.o

bubblesort.o: bubblesort.cpp taxpayer.h
	$(CC) -c bubblesort.cpp

clean: 
	rm *.o myhie myfifo heapsort bubblesort