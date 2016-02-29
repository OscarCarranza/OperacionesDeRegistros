#include "record.h"
#include <stdio.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

RecordFile::RecordFile(){
	fileBooks = fopen("Libros.bin", "ab");
	fileEds = fopen("Editoriales.bin", "ab");
}

RecordFile::~RecordFile(){
	fclose(fileBooks);
	fclose(fileEds);
}

void RecordFile::writeRecord(int isbn, char name[20], int id_editorial, char author[20]){
	fileBooks = fopen("Libros.bin","ab");
	fwrite(name,20,1,fileBooks);
	fwrite(&isbn,4,1,fileBooks);
	fwrite(author,20,1,fileBooks);
	fwrite(&id_editorial,4,1,fileBooks);
}

void RecordFile::writeRecord(int id, char name[20], char adress[20]){
	fileEds = fopen("Editoriales.bin", "ab");
	fwrite(name, 20, 1, fileEds);
	fwrite(&id, 4, 1, fileEds);
	fwrite(adress,20,1,fileEds);	
}

void RecordFile::listRecords(int type){

	if(type == 1){ //books

		fileBooks = fopen("Libros.bin","rb");
		fseek(fileBooks,0,SEEK_END);
		int number_registers = ftell(fileBooks)/48;
	
		fseek(fileBooks,0,SEEK_SET);
		char name[20];
		char author[20];
		int isbn;
		int id_editorial;

		int current = 0;
		while(current < number_registers){
			fread(name,20,1,fileBooks);
			fread(&isbn,4,1,fileBooks);
			fread(author,20,1,fileBooks);
			fread(&id_editorial,4,1,fileBooks);
			cout << "ISBN: " << isbn << ", Name: " << name << ", Author: " << author << ", ID Editorial: " << id_editorial << endl;
			current++;
		}
	}

	else{ //editorials

		fileEds = fopen("Editoriales.bin","rb");
		fseek(fileEds,0,SEEK_END);
		int number_registers = ftell(fileEds)/44;
		
		fseek(fileEds,0,SEEK_SET);
		char name[20];
		char adress[20];
		int id;

		int current = 0;
		while(current < number_registers){
			fread(name, 20, 1, fileEds);
			fread(&id, 4, 1, fileEds);
			fread(adress,20,1,fileEds);
			cout << "ID: " << id << ", Name: " << name << ", Adress: " << adress << endl;
			current++;
		}
	}
	
}
	
void RecordFile::readRecord(int rrnORaccess, int type){

	if(type == 1){ // read book
		fileBooks = fopen("Libros.bin","rb");
		fseek(fileBooks,(rrnORaccess-1)*48,SEEK_SET);
		char name[20];
		char author[20];
		int isbn;
		int id_editorial;

		fread(name,20,1,fileBooks);
		fread(&isbn,4,1,fileBooks);
		fread(author,20,1,fileBooks);
		fread(&id_editorial,4,1,fileBooks);
		cout << "ISBN: " << isbn << ", Name: " << name << ", Author: " << author << ", ID Editorial: " << id_editorial;
	}

	else{ // access editorial

		fseek(fileBooks,0,SEEK_END);
		int number_registers = ftell(fileBooks)/48;

		fileBooks = fopen("Libros.bin","rb");
		fseek(fileBooks,0,SEEK_SET);
		char name[20];
		char author[20];
		int isbn;
		int id_editorial;

		int current = 0;
		bool found = false;
		while(current < number_registers){
			fread(name,20,1,fileBooks);
			fread(&isbn,4,1,fileBooks);
			fread(author,20,1,fileBooks);
			fread(&id_editorial,4,1,fileBooks);
			
			if(id_editorial == rrnORaccess){
				cout << "ISBN: " << isbn << ", Name: " << name << ", Author: " << author << ", ID Editorial: " << id_editorial << endl;
				found = true;
			}
			current++;
		}

		if(!found){
			cout << "Sorry, there are no books in this editorial";
		}
	}	
}

void RecordFile::deleteRecord(int rrn, int type){
	fileBooks = fopen("Libros.bin","ab");
	int number_registers = ftell(fileBooks)/48;
	fseek(fileBooks,(rrn-1)*48,SEEK_SET);
	char mark[2] = {'*','*'};
	ftell(fileBooks);
	fwrite(mark,2,1,fileBooks);
}