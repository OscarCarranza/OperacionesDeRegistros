#include "record.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::flush;


struct Libro
{
	char name[20];
	char author[20];
	int isbn;
	int id_editorial;
};

struct Editorial
{
	char name[20];
	char adress[20];
	int id_editorial;
};

int main(int argc, char const *argv[]){

	int option;
	bool enter = true;
	int option2;
	RecordFile rf;

	while(enter){

		cout << "\n---- Book & Editorial Control ----\n\n";
		cout << "    1. Book Management \n    2. Editorial Management \n    3. Exit \n\nEnter option: ";
		cin >> option2;
		

		if(option2 == 1){
			cout << "\n    1. Add Book \n    2. Read Book \n    3. List books \n    4. Delete book \n    5. Update book \n\n";
			cout << "Choose an option: ";
			cin >> option;
			Libro book;
			

			if (option == 1){
				
				cout << "Name of the book: ";
				cin >> book.name;
				cout << "ISBN: ";
				cin >> book.isbn;
				cout << "Author: ";
				cin >> book.author;
				cout << "Editorial ID: ";
				cin >> book.id_editorial;
				rf.writeRecord(book.isbn, book.name, book.id_editorial, book.author);
			}
			
			else if(option == 2){
				int rrn;
				cout << "Insert RRN: ";
				cin >> rrn;
				rf.readRecord(rrn,1);

			}

			else if(option == 3){
				rf.listRecords(1);
			}

			else if(option == 4){
				int rrn, valid;
				cout << "Remove Register #: ";
				cin >> rrn;
				cout << "Are you sure? [1. YES/ 2. NO]: ";
				cin >> valid;

				if(valid == 1)
					rf.deleteRecord(rrn,1);
			}

			else if(option == 5){
				int rrn;
				cout << "Update Register #: ";
				cin >> rrn;
				rf.updateRecord(rrn,1);
			}

			else{
				cout << "Invalid option";
			}

		} //book control

		else if(option2 == 2){
		   cout << "\n    1. New Editorial\n    2. Remove Editorial\n    3. List Editorials \n    4. Access Editorial \n    5. Update Editorial\n\n";
		   cout << "Enter option: ";
		   cin >> option;

		   if (option == 1){
				Editorial ed;
				cout << "Name of Editorial: ";
				cin >> ed.name;
				cout << "Editorial ID: ";
				cin >> ed.id_editorial;
				cout << "Adress: ";
				cin >> ed.adress;
				rf.writeRecord(ed.id_editorial,ed.name,ed.adress);
			}

			if(option == 2){
				int rrn;
				cout << "Remove Editorial #: ";
				cin >> rrn;
				rf.deleteRecord(rrn,2);
			}
		
			else if(option == 4){
				int access;
				cout << "Enter ID Editorial #: ";
				cin >> access;
				rf.readRecord(access,2);
			}

			else if(option == 3){
				rf.listRecords(2);
			}

			else if(option == 5){
				int rrn;
				cout << "Update Editorial #: ";
				cin >> rrn;
				rf.updateRecord(rrn,2);
			}

			else{
				cout << "Invalid option";
			}

		} //editorials

		else if(option2 == 3){
			enter = false;
		}

		else{
			cout << "Invalid option!";
		}

	}

	cout << "Your session has closed\n";

	return 0;
}
