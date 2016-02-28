#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>

using std::cout;
using std::cin;
using std::vector;
using std::string;
using std::flush;


struct Libro
{
	char name[20];
	char author[20];
	char isbn[5];
	char id_editorial[5];
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
		void writeRecord(char*);
		char* readRecord(int);
		char* merge(char*,char*,char*,char*);
};

RecordFile::RecordFile(){
	file = fopen("Libros.bin", "ab");
}

RecordFile::~RecordFile(){
	fclose(file);
}

void RecordFile::writeRecord(char* data){
	fwrite(data,50,1,file);
}
	
char* RecordFile::readRecord(int rrn){

}

char* RecordFile::merge(char* name, char* isbn, char* author, char* id){
	char data[50];
	strcpy(data,name);
	strcat(data,",");
	strcat(data,isbn);
	strcat(data,",");
	strcat(data,author);
	strcat(data,",");
	strcat(data,id);
}

int main(int argc, char const *argv[]){

	int option = 0;
	bool enter = true;

	while(enter){
		cout << "---- Book & Editorial Control ----\n\n";
		cout << "    1. Add Book \n    2. Read Book \n    3. List books \n    4. Delete book \n    5. Update book \n    5.New Editorial\n    6. Remove Editorial\n    7. List Editorials \n    8. Access Editorial \n\n";
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
			char* data = rf.merge(book.name,book.isbn,book.author,book.id_editorial);
			rf.writeRecord(data);
		}
		
		/*else if(option == 2){

			char name[20];
			char author[20];
			int isbn;
			int id_editorial;

			FILE* file = fopen("Libros.bin", "rb");
			fseek(file,0,SEEK_SET);
			while(!feof(file)){
				fread(&isbn,4,1,file);
				fread(name,20,1,file);
				fread(&id_editorial,4,1,file);
				fread(author,20,1,file);
				cout << "Book name: " << name << "   Author: " << author << "   ISBN: " << isbn << "   Editorial ID: " << id_editorial << "\n";
			}
			fclose(file);

		}*/

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
