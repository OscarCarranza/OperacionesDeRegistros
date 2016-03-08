#include "record.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <limits>

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
	FILE* indiceLibros = fopen("IndexLibros.bin","ab");
	FILE* indiceEds = fopen("IndexEditoriales.bin","ab");

	fseek(indiceLibros,0,SEEK_END);
	if(ftell(indiceLibros) == 0){
		char info[] = "int LLAVE,int RRN";
		fwrite(info,sizeof(info),1,indiceLibros);
	}

	fseek(indiceEds,0,SEEK_END);
	if(ftell(indiceEds) == 0){
		char info[] = "int LLAVE,int RRN";
		fwrite(info,sizeof(info),1,indiceEds);
	}

	fseek(indiceLibros,0,SEEK_END);
	int num_books = (ftell(indiceLibros)-18)/8;
	fseek(indiceEds,0,SEEK_END);
	int num_eds = (ftell(indiceEds)-18)/8;
	
	indiceLibros = fopen("IndexLibros.bin","rb");
	int x,y;
	fseek(indiceLibros,18,SEEK_SET);

	for(int i = 0; i < num_books; i++){
		fread(&x,4,1,indiceLibros);
		fread(&y,4,1,indiceLibros);
		indexKey.key = x;
		indexKey.rrn = y;
		indexBooks.push_back(indexKey);
	}

	indiceEds = fopen("IndexEditoriales.bin","rb");
	fseek(indiceEds,18,SEEK_SET);
	for(int i = 0; i < num_eds; i++){
		fread(&x,4,1,indiceEds);
		fread(&y,4,1,indiceEds);
		indexKey.key = x;
		indexKey.rrn = y;
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
				cout << "ISBN[4]: ";
				book.isbn = rf.validNumber(1);
				cout << "Author: ";
				cin >> book.author;
				cout << "Editorial ID[4]: ";
				book.id_editorial = rf.validNumber(2);

				//check isbn on vector
				bool exists = false;

				for(int i = 0; i < indexBooks.size(); i++){
					if(indexBooks[i].key == book.isbn){
						cout << "SORRY :( this ISBN already exists" << endl;
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
				int isbn;
				cout << "Insert ISBN: ";
				isbn = rf.validNumber(1);

				int i = 0;
				int left = 0;
				int right = indexBooks.size()-1;
				int mid;
				bool found = false;

				while (left <= right) {
					i++;
					mid = (int) ((left + right) / 2);
					if (isbn == indexBooks[mid].key){ 
						cout << "Book found!" << endl;
						int locOfBook = indexBooks[mid].rrn;
						rf.readRecord(locOfBook,1);
						found = true;
						break;
					}	
					else if (isbn > indexBooks[mid].key)
						left = mid + 1;
					else
						right = mid - 1;
				}

				if(found == false){
					cout << "Book not found :(" << endl;
				}

			}

			else if(option == 3){
				rf.listRecords(1);
			}

			else if(option == 4){
				int isbn, valid;
				cout << "Remove ISBN: ";
				isbn = rf.validNumber(1);
				
				cout << "Are you sure? [1. YES/ 2. NO]: ";
				valid = rf.validNumber(3);

				if(valid == 1){

					int i = 0;
					int left = 0;
					int right = indexBooks.size()-1;
					int mid;
					bool found = false;
					int loc;

					while (left <= right) {
						i++;
						mid = (int) ((left + right) / 2);
						if (isbn == indexBooks[mid].key){ 
							cout << "Book found!" << endl;
							int locOfBook = indexBooks[mid].rrn;
							loc = rf.deleteRecord(locOfBook,1);
							found = true;
							break;
						}	
						else if (isbn > indexBooks[mid].key)
							left = mid + 1;
						else
							right = mid - 1;
					}

					if(found == false){
						cout << "Book not found :(" << endl;
					}

					//erase from vector
					for(int i = 0; i < indexBooks.size(); i++){
						if(indexBooks[i].key == loc){
							indexBooks.erase(indexBooks.begin()+i);
						}
					}
				}
			}

			else if(option == 5){
				int isbn;
				cout << "Update ISBN: ";
				isbn = rf.validNumber(1);

				int i = 0;
				int left = 0;
				int right = indexBooks.size()-1;
				int mid;
				bool found = false;

				while (left <= right) {
					i++;
					mid = (int) ((left + right) / 2);
					if (isbn == indexBooks[mid].key){ 
						cout << "Book found!" << endl;
						int locOfBook = indexBooks[mid].rrn;
						rf.updateRecord(locOfBook,1);
						found = true;
						break;
					}	
					else if (isbn > indexBooks[mid].key)
						left = mid + 1;
					else
						right = mid - 1;
				}

				if(found == false){
					cout << "Book not found :(" << endl;
				}
			}

			else if(option == 6){
				cout << "PUEDE SER QUE EL INDICE ESTE SUCIO! \nEN ESTE CASO CIERRE EL PROGRAMA Y VUELVA A CORRERLO :D\n\n";
				
				for(int i = 0; i< indexBooks.size(); i++){
					cout << indexBooks[i].key << " | " << indexBooks[i].rrn << endl;
				}
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
				ed.id_editorial = rf.validNumber(2);
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
					indexEds.push_back(indexKey);
				}
			}

			else if(option == 2){
				int id;
				int valid;
				int loc;
				cout << "Remove ID: ";
				id = rf.validNumber(2);
				cout << "Are you sure? [1. YES/ 2. NO]: ";
				valid = rf.validNumber(3);

				if(valid == 1){
					int i = 0;
					int left = 0;
					int right = indexEds.size()-1;
					int mid;
					bool found = false;

					while (left <= right) {
						i++;
						mid = (int) ((left + right) / 2);
						if (id == indexEds[mid].key){ 
							cout << "Editorial found!" << endl;
							int locOfEd = indexEds[mid].rrn;
							loc = rf.deleteRecord(locOfEd,2);
							found = true;
							break;
						}	
						else if (id > indexEds[mid].key)
							left = mid + 1;
						else
							right = mid - 1;
					}

					if(found == false){
						cout << "Editorial not found :(" << endl;
					}

					//erase from vector
					for(int i = 0; i < indexEds.size(); i++){
						if(indexEds[i].key == loc){
							indexEds.erase(indexEds.begin()+i);
						}
					}
				}

			}
		
			else if(option == 4){
				int access;
				cout << "Enter ID Editorial #: ";
				access = rf.validNumber(2);
				rf.readRecord(access,2);
			}

			else if(option == 3){
				rf.listRecords(2);
			}

			else if(option == 5){
				int id;
				cout << "Insert ID: ";
				id = rf.validNumber(2);

				int i = 0;
				int left = 0;
				int right = indexEds.size()-1;
				int mid;
				bool found = false;

				while (left <= right) {
					i++;
					mid = (int) ((left + right) / 2);
					if (id == indexEds[mid].key){ 
						cout << "Editorial found!" << endl;
						int locOfEd = indexEds[mid].rrn;
						rf.updateRecord(locOfEd,2);
						found = true;
						break;
					}	
					else if (id > indexEds[mid].key)
						left = mid + 1;
					else
						right = mid - 1;
				}

				if(found == false){
					cout << "Editorial not found :(" << endl;
				}
			}

			else if(option == 6){
				cout << "PUEDE SER QUE EL INDICE ESTE SUCIO! \nEN ESTE CASO CIERRE EL PROGRAMA Y VUELVA A CORRERLO :D\n\n";
				
				for(int i = 0; i< indexEds.size(); i++){
					cout << indexEds[i].key << " | " << indexEds[i].rrn << endl;
				}
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

		for(int i = 0; i < indexBooks.size()-1; i++){
			for(int j = 0; j < indexBooks.size()-1; j++){
				if(indexBooks[j].key > indexBooks[j+1].key){
					Ref temp = indexBooks[j];
					indexBooks[j] = indexBooks[j+1];
					indexBooks[j+1] = temp;
				}
			}
		}	
		

		indiceLibros = fopen("IndexLibros.bin", "r+b");
		fseek(indiceLibros,18,SEEK_SET);
		int rrn,key;

		int curr = 0;
		while(curr < indexBooks.size()){
			key = indexBooks[curr].key;
			fwrite(&key,4,1,indiceLibros);
			rrn = indexBooks[curr].rrn;
			fwrite(&rrn,4,1,indiceLibros);
			curr++;
		}
	}

	if(dB2 == 1){
		//order vector

		for(int i = 0; i < indexEds.size()-1; i++){
			for(int j = 0; j < indexEds.size()-1; j++){
				if(indexEds[j].key > indexEds[j+1].key){
					Ref temp = indexEds[j];
					indexEds[j] = indexEds[j+1];
					indexEds[j+1] = temp;
				}
			}
		}	
		

		indiceEds = fopen("IndexEditoriales.bin", "r+b");
		fseek(indiceEds,18,SEEK_SET);
		int rrn,key;

		int curr = 0;
		while(curr < indexEds.size()){
			key = indexEds[curr].key;
			fwrite(&key,4,1,indiceEds);
			rrn = indexEds[curr].rrn;
			fwrite(&rrn,4,1,indiceEds);
			curr++;
		}
	}

	fflush(indiceLibros);
	fclose(indiceLibros);
	fflush(indiceEds);
	fclose(indiceEds);
	cout << "Your session has closed\n";
	return 0;
}
