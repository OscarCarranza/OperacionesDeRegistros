exe:	record.o main.o
	g++ record.o main.o -o exe
record.o:	record.h record.cpp
	g++ -c record.cpp
main.o:	record.o main.cpp
	g++ -c main.cpp
