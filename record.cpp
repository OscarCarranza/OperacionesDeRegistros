#include "record.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <string.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;

RecordFile::RecordFile(){
	bool BooksExist = fileBooksExists();
	bool EdsExist = fileEdsExists();
	int availList = -1;
	bool dirtyBit = 0;

	if(!BooksExist){

		fileBooks = fopen("Libros.bin", "ab");
		char info[] = "int ISBN,char Name[20],int ID_editorial,char Author[20]";	
		fwrite(&availList,4,1,fileBooks);
		fwrite(&dirtyBit,1,1,fileBooks);
		fwrite(info, sizeof(info),1,fileBooks);
		fflush(fileBooks);
	}
	
	if(!EdsExist){
		fileEds = fopen("Editoriales.bin", "ab");
		char info[]= "char Name[20], int ID_editorial, char Adress[20]";
		fwrite(&availList,1,1,fileEds);
		fwrite(&dirtyBit,1,1,fileEds);
		fwrite(info, sizeof(info),1,fileEds);
		fflush(fileEds);
	}
	
}

RecordFile::~RecordFile(){

	bool BooksExist = fileBooksExists();
	bool EdsExist = fileEdsExists();

	if(!BooksExist)
		fclose(fileBooks);
	if(!EdsExist)
		fclose(fileEds);
}

void RecordFile::writeRecord(int isbn, char name[20], int id_editorial, char author[20]){

	fileBooks = fopen("Libros.bin","rb");	
	fseek(fileBooks,0,SEEK_SET);
	int avl;
	fread(&avl,4,1,fileBooks);

	if(avl == -1){
		fileBooks = fopen("Libros.bin","ab");		
		fseek(fileBooks,0,SEEK_END);	
		fwrite(name,20,1,fileBooks);
		fwrite(&isbn,4,1,fileBooks);
		fwrite(author,20,1,fileBooks);
		fwrite(&id_editorial,4,1,fileBooks);
	}

	else{
		fileBooks = fopen("Libros.bin","r+b");
		int location = 61 + ((avl-1)*48);
		int value_avl;
		fseek(fileBooks,location+1,SEEK_SET);

		// Leo el elemento siguiente del availist
		fread(&value_avl,4,1,fileBooks); 
		
		//escribo el regsitro
		fseek(fileBooks,location,SEEK_SET); 
		fwrite(name,20,1,fileBooks); 
		fwrite(&isbn,4,1,fileBooks);
		fwrite(author,20,1,fileBooks);
		fwrite(&id_editorial,4,1,fileBooks);

		//escribo nueva pos dicponible en el archivo
		fseek(fileBooks,0,SEEK_SET);
		fwrite(&value_avl,4,1,fileBooks);

	}
	fflush(fileBooks);
	cout << "Record Succesfully Added! :D" << endl;	
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
		int number_registers = (ftell(fileBooks)-61)/48;
	
		fseek(fileBooks,61,SEEK_SET);
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
			if(name[0] != '*'){
				cout << " " << isbn << " | " << name << " | " << author << " | ED " << id_editorial << endl;
			}
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
			if(name[0] != '*'){
				cout << "ID: " << id << ", Name: " << name << ", Adress: " << adress << endl;
			}	
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

	if(type == 1){
		int location = 61 + (rrn-1)*48;
		fileBooks = fopen("Libros.bin","r+b");
		int number_registers = ftell(fileBooks)/48;
		fseek(fileBooks,location,SEEK_SET);
		cout << ftell(fileBooks);
		char mark[1] = {'*'};
		fwrite(mark,1,1,fileBooks);

		int avl;
		fseek(fileBooks,0,SEEK_SET);
		fread(&avl,4,1,fileBooks); //primer elemento availList 
		fseek(fileBooks,0,SEEK_SET); //ya estoy en el primer byte
		fwrite(&rrn,4,1,fileBooks); 
		fseek(fileBooks,location+1,SEEK_SET);
		fwrite(&avl,4,1,fileBooks); 
		fflush(fileBooks);

	}

	else if(type == 2){
		fileEds = fopen("Libros.bin","r+b");
		int number_registers = ftell(fileEds)/44;
		fseek(fileBooks,(rrn-1)*44,SEEK_SET);
		cout << ftell(fileEds);
		char mark[1] = {'*'};
		fwrite(mark,1,1,fileEds);
	}
	
}

void RecordFile::updateAvailList(int deletedRegister, int type){
	if(type == 1){

	}
	else{

	}
}


bool RecordFile::fileBooksExists(){
	ifstream ifile("Libros.bin");
	return ifile;
}

bool RecordFile::fileEdsExists(){
	ifstream ifile("Editoriales.bin");
	return ifile;
}

