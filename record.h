#ifndef  RECORD_H
#define RECORD_H 
#include <stdio.h>

class RecordFile{
		FILE* fileBooks;
		FILE* fileEds;
	public: 
		RecordFile();
		~RecordFile();
		void writeRecord(int, char[20], int, char[20]); // New book
		void writeRecord(int, char[20], char[20]);      // New Editorial
		void readRecord(int,int); // int1 = rrn, int2 = type(editorial or book)
		void listRecords(int); //editorial or book
		void deleteRecord(int,int);
		void updateAvailList(int, int);
		bool fileEdsExists();
		bool fileBooksExists();
};

#endif


