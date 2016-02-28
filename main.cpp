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

class RecordFile{
		FILE* file;
	public: 
		RecordFile();
		~RecordFile();
		void writeRecord(int, char[20], int, char[20]);
		void readRecord(int);
		void listRecords();
};

RecordFile::RecordFile(){
	file = fopen("Libros.bin", "ab");
}

RecordFile::~RecordFile(){
	fclose(file);
}

void RecordFile::writeRecord(int isbn, char name[20], int id_editorial, char author[20]){
	file = fopen("Libros.bin","ab");
	fwrite(&isbn,4,1,file);
	fwrite(name,20,1,file);
	fwrite(&id_editorial,4,1,file);
	fwrite(author,20,1,file);
}

void RecordFile::listRecords(){

	fseek(file,0,SEEK_END);
	int number_registers = ftell(file)/48;

	file = fopen("Libros.bin","rb");
	fseek(file,0,SEEK_SET);
	char name[20];
	char author[20];
	int isbn;
	int id_editorial;

	int current = 0;
	while(current < number_registers){
			fread(&isbn,4,1,file);
			fread(name,20,1,file);
			fread(&id_editorial,4,1,file);
			fread(author,20,1,file);
			cout << "ISBN: " << isbn << ", Name: " << name << ", Author: " << author << ", ID Editorial: " << id_editorial << endl;
			current++;
	}
}
	
void RecordFile::readRecord(int rrn){
	file = fopen("Libros.bin","rb");
	fseek(file,(rrn-1)*50,SEEK_SET);
	char name[20];
	char author[20];
	int isbn;
	int id_editorial;

	fread(&isbn,4,1,file);
	fread(name,20,1,file);
	fread(&id_editorial,4,1,file);
	fread(author,20,1,file);
	cout << "ISBN: " << isbn << ", Name: " << name << ", Author: " << author << ", ID Editorial: " << id_editorial;
}

int main(int argc, char const *argv[]){

	int option = 0;
	bool enter = true;

	while(enter){
		cout << "---- Book & Editorial Control ----\n\n";
		cout << "    1. Add Book \n    2. Read Book \n    3. List books \n    4. Delete book \n    5. Update book \n    6. New Editorial\n    7. Remove Editorial\n    8. List Editorials \n    9. Access Editorial \n\n";
		cout << "Choose an option: ";
		cin >> option;
		Libro book;
		RecordFile rf;

		if (option == 1){
			
			cout << "Name of the book: ";
			cin >> book.name;
			cout << "ISBN: ";
			cin >> book.isbn;
			cout << "Author: ";
			cin >> book.author;
			cout << "Editorial ID: ";
			cin >> book.id_editorial;
			cout << book.name << "   " << book.isbn << "    " << book.author << "   " << book.id_editorial << endl;
			rf.writeRecord(book.isbn, book.name, book.id_editorial, book.author);
		}
		
		else if(option == 2){
			int rrn;
			cout << "Insert RRN: ";
			cin >> rrn;
			rf.readRecord(rrn);

		}

		else if(option == 3){
			rf.listRecords();
		}

		else if (option == 5){
			Editorial ed;
			cout << "Name of Editorial: ";
			cin >> ed.name;
			cout << "Editorial ID: ";
			cin >> ed.id_editorial;
			cout << "Adress: ";
			cin >> ed.adress;

			ed.name[19] = '\0';
			FILE* file = fopen("Editoriales.bin", "ab");
			fwrite(book.name, sizeof(book.name), 1, file);
			fwrite(&book.id_editorial, sizeof(book.id_editorial), 1, file);
			fclose(file);
		}
		
		else if(option == 7){

			/*FILE* file = fopen("Editoriales.bin", "rb");
			fseek(file,0,SEEK_SET);
			while(!feof(file)){
				fread(name,20,1,file);
				fread(&id_editorial,4,1,file);
				fread(adress,20,1,file);
				cout << "Editorial: " << name << "     ID: " << isbn << "    Adress: " << adress << "\n";
			}
			fclose(file);*/

		}

		int n ;
		cout << "Back to menu? [1.YES/2.NO]: ";
		cin >> n;
		if(n == 2){
			enter = false;
		}
		cout << "\n";
	}

	cout << "Your session has closed\n";

	return 0;
}
