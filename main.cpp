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

struct Ref
{
	int rrn;
	int key;
};

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
	Ref indexKey;
	vector <Ref> indexBooks;
	vector <Ref> indexEds;

	//llenar el vector con lo que tengo en el indice;
	FILE* indiceLibros = rf.getIndexBooks();
	FILE* indiceEds = rf.getIndexEds();
	indiceLibros = fopen("IndexLibros.bin","rb");
	indiceEds = fopen("IndexEditoriales.bin","rb");

	fseek(indiceLibros,0,SEEK_END);
	int num_books = (ftell(indiceLibros)-17)/8;
	cout << "num_books: " << num_books;
	fseek(indiceEds,0,SEEK_END);
	int num_eds = (ftell(indiceEds)-17)/8;

	fseek(indiceLibros,17,SEEK_SET);
	int temp;
	for(int i = 0; i < num_books; i++){
		fread(&temp,4,1,indiceLibros);
		indexKey.key = temp;
		fread(&temp,4,1,indiceLibros);
		indexKey.rrn = temp;
		indexBooks.push_back(indexKey);
	}

	fseek(indiceEds,17,SEEK_SET);
	for(int i = 0; i < num_eds; i++){
		fread(&indexKey.rrn,4,1,indiceLibros);
		fread(&indexKey.key,4,1,indiceLibros);
		indexEds.push_back(indexKey);
	}
	
	while(enter){

		cout << "\n---- Book & Editorial Control ----\n\n";
		cout << "    1. Book Management \n    2. Editorial Management \n    3. Crossed List Inventory \n    4. Exit \n\nEnter option: ";
		cin >> option2;
		

		if(option2 == 1){
			cout << "\n    1. Add Book \n    2. Read Book \n    3. List books \n    4. Delete book \n    5. Update book\n    6. Print Index\n\n";
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

				//check isbn on vector
				bool exists = false;

				for(int i = 0; i < indexBooks.size(); i++){
					if(indexBooks[i].key == book.isbn){
						cout << "SORRY :( ,this ISBN already exists" << endl;
						exists = true;
					}
				}

				if(exists == false){
					int loc = rf.writeRecord(book.isbn, book.name, book.id_editorial, book.author);
					
					//Add to vector
					indexKey.key = book.isbn;
					indexKey.rrn = loc;
					indexBooks.push_back(indexKey);
				}
				
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

				if(valid == 1){
					int id = rf.deleteRecord(rrn,1);
					for(int i = 0; i < indexBooks.size(); i++){
						if(indexBooks[i].key == id){
							indexBooks.erase(indexBooks.begin()+i);
						}
					}
				}
			}

			else if(option == 5){
				int rrn;
				cout << "Update Register #: ";
				cin >> rrn;
				rf.updateRecord(rrn,1);
			}

			else if(option == 6){
				rf.printIndex(1);
			}

			else{
				cout << "Invalid option";
			}

		} //book control

		else if(option2 == 2){
		   cout << "\n    1. New Editorial\n    2. Remove Editorial\n    3. List Editorials \n    4. Access Editorial \n    5. Update Editorial\n    6. Print Index\n\n";
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

				//check id on vector
				bool exists = false;

				for(int i = 0; i < indexEds.size(); i++){
					if(indexEds[i].key == ed.id_editorial){
						cout << "SORRY :( ,this ID already exists" << endl;
						exists = true;
					}
				}

				if(exists == false){
					int loc = rf.writeRecord(ed.id_editorial,ed.name,ed.adress);
					
					//Add to vector
					indexKey.key = ed.id_editorial;
					indexKey.rrn = loc;
					indexBooks.push_back(indexKey);
				}
			}

			else if(option == 2){
				int rrn;
				int valid;
				cout << "Remove Editorial #: ";
				cin >> rrn;
				cout << "Are you sure? [1. YES/ 2. NO]: ";
				cin >> valid;

				if(valid == 1){
					int id = rf.deleteRecord(rrn,2);
					for(int i = 0; i < indexBooks.size(); i++){
						if(indexEds[i].key == id){
							indexEds.erase(indexEds.begin()+i);
						}
					}
				}

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

			else if(option == 6){
				rf.printIndex(2);
			}

			else{
				cout << "Invalid option";
			}

		} //editorials

		else if(option2 == 4){
			enter = false;
		}

		else if(option2 == 3){
			rf.crossList();
		}

		else{
			cout << "Invalid option!";
		}

	}

	//update index
	bool dB1 = rf.dirtyBitBooks();
	bool dB2 = rf.dirtyBitEds();

	if(dB1 == 1){

		//order vector
		vector <Ref> orderedVector;
		for(int i = 0; i < indexBooks.size(); i++){
			int menor = indexBooks[i].key;
			int pos = i;
			for(int j = 0; j < indexBooks.size()-1; j++){
				if(menor > indexBooks[j+1].key){
					pos = j+1;
				}
			}
			orderedVector.push_back(indexBooks[pos]);	
			indexBooks.erase(indexBooks.begin()+pos);
			if(i == indexBooks.size()-1)
				orderedVector.push_back(indexBooks[0]);
		}	
				
		indiceLibros = fopen("IndexLibros.bin", "r+b");
		fseek(indiceLibros,17,SEEK_SET);
		cout << "vector size: " << orderedVector.size() << endl;
		int temp;
		for(int i = 0; i < orderedVector.size(); i++){
			temp = orderedVector[i].key;
			cout << temp << endl;
			fwrite(&temp,4,1,indiceLibros);
			temp = orderedVector[i].rrn;
			cout << temp << endl;
			fwrite(&temp,4,1,indiceLibros);
		}
		fflush(indiceLibros);
	}

	if(dB1 == 2){

		//order vector
		vector <Ref> orderedVector;
		for(int i = 0; i < indexEds.size(); i++){
			int menor = indexEds[i].key;
			int pos = i;
			for(int j = 0; j < indexEds.size()-1; j++){
				if(menor > indexEds[j+1].key){
					pos = j+1;
				}
			}
			orderedVector.push_back(indexEds[pos]);	
			indexBooks.erase(indexEds.begin()+pos);
			if(i == indexEds.size()-1)
				orderedVector.push_back(indexEds[0]);
		}	
				
		indiceEds = fopen("IndexEditoriales.bin", "r+b");
		fseek(indiceEds,17,SEEK_SET);
		cout << "vector size: " << orderedVector.size() << endl;
		int temp;
		for(int i = 0; i < orderedVector.size(); i++){
			temp = orderedVector[i].key;
			cout << temp << endl;
			fwrite(&temp,4,1,indiceLibros);
			temp = orderedVector[i].rrn;
			cout << temp << endl;
			fwrite(&temp,4,1,indiceLibros);
		}
		fflush(indiceEds);
	}



	cout << "Your session has closed\n";

	return 0;
}
