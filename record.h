#ifndef  RECORD_H
#define RECORD_H 
#include <stdio.h>

class RecordFile{
		FILE* fileBooks;
		FILE* fileEds;
		FILE* indexBooks;
		FILE* indexEds;
	public: 
		//Records
		RecordFile();
		~RecordFile();
		int writeRecord(int, char[20], int, char[20]); // New book
		int writeRecord(int, char[20], char[20]);      // New Editorial
		void readRecord(int,int); // int1 = rrn, int2 = type(editorial or book)
		void listRecords(int); //editorial or book
		int deleteRecord(int,int);
		void updateRecord(int,int);
		bool fileEdsExists();
		bool fileBooksExists();
		void crossList();

		//Indexes
		bool indexEdsExists();
		bool indexBooksExists();
		FILE* getIndexBooks();
		FILE* getIndexEds();
		bool dirtyBitBooks();
		bool dirtyBitEds();
		void printIndex(int);

};

#endif


