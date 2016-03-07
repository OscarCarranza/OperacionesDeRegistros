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
		char info[]= "char Name[20],int ID_editorial,char Adress[20]";
		fwrite(&availList,4,1,fileEds);
		fwrite(&dirtyBit,1,1,fileEds);
		fwrite(info, sizeof(info),1,fileEds);
		fflush(fileEds);
	}

	//indice
	BooksExist = indexBooksExists();
	EdsExist = indexEdsExists();

	if(!BooksExist){
		indexBooks = fopen("IndexLibros.bin", "ab");
		char info[] = "int LLAVE,int RRN";
		fwrite(info,17,1,indexBooks);
		fflush(indexBooks);
	}
	
	if(!EdsExist){
		indexEds = fopen("IndexEditoriales.bin", "ab");
		char info[] = "int LLAVE,int RRN";
		fwrite(info,17,1,indexEds);
		fflush(indexEds);
	}
	
}

RecordFile::~RecordFile(){

	bool BooksExist = fileBooksExists();
	bool EdsExist = fileEdsExists();
	bool indexBexists = indexBooksExists();
	bool indexEexists = indexEdsExists();

	bool dB = 0;
	fileBooks = fopen("Libros.bin","r+b");
	fileEds = fopen("Editoriales.bin","r+b");
	fseek(fileBooks,5,SEEK_SET);
	fseek(fileEds,5,SEEK_SET);
	fwrite(&dB,1,1,fileBooks);
	fwrite(&dB,1,1,fileEds);

	if(!BooksExist)
		fclose(fileBooks);
	if(!EdsExist)
		fclose(fileEds);
	if(!indexBexists)
		fclose(indexBooks);
	if(!indexEexists)
		fclose(indexEds);
}

int RecordFile::writeRecord(int isbn, char name[20], int id_editorial, char author[20]){

	fileBooks = fopen("Libros.bin","r+b");	
	fseek(fileBooks,0,SEEK_SET);
	int avl;
	bool dB = 1;
	bool temp;
	fread(&avl,4,1,fileBooks);
	int rrn;

	//marcamos dB
	fseek(fileBooks,5,SEEK_SET);
	fwrite(&dB,1,1,fileBooks);
	fseek(fileBooks,5,SEEK_SET);
	fread(&temp,1,1,fileBooks);

	if(avl == -1){
		fileBooks = fopen("Libros.bin","ab");		
		fseek(fileBooks,0,SEEK_END);	
		rrn = (ftell(fileBooks)-61)/48;
		fwrite(name,20,1,fileBooks);
		fwrite(&isbn,4,1,fileBooks);
		fwrite(author,20,1,fileBooks);
		fwrite(&id_editorial,4,1,fileBooks);
	}

	else{
		int location = 61 + ((avl-1)*48);
		int value_avl;
		fseek(fileBooks,location+1,SEEK_SET);

		// Leo el elemento siguiente del availist
		fread(&value_avl,4,1,fileBooks); 
		
		//escribo el regsitro
		fseek(fileBooks,location,SEEK_SET); 
		rrn = (ftell(fileBooks)-61)/48;
		fwrite(name,20,1,fileBooks); 
		fwrite(&isbn,4,1,fileBooks);
		fwrite(author,20,1,fileBooks);
		fwrite(&id_editorial,4,1,fileBooks);

		//escribo nueva pos dicponible en el archivo
		fseek(fileBooks,0,SEEK_SET);
		fwrite(&value_avl,4,1,fileBooks);

	}
	fflush(fileBooks);
	cout << "Book Succesfully Added! :D" << endl;	
	return rrn;
}

int RecordFile::writeRecord(int id, char name[20], char adress[20]){

	fileEds = fopen("Editoriales.bin","r+b");	
	fseek(fileEds,0,SEEK_SET);
	int avl;
	bool dB = 1;
	fread(&avl,4,1,fileEds);
	int rrn;

	if(avl == -1){
		fileEds = fopen("Editoriales.bin","ab");		
		fseek(fileEds,0,SEEK_END);	
		rrn = (ftell(fileBooks)-61)/48;
		fwrite(name, 20,1, fileEds);
		fwrite(&id, 4,1, fileEds);
		fwrite(adress,20,1,fileEds);		
	}

	else{
		int location = 52 + ((avl-1)*44);
		int value_avl;
		fseek(fileEds,location+1,SEEK_SET);

		// Leo el elemento siguiente del availist
		fread(&value_avl,4,1,fileEds); 
		
		//escribo el regsitro
		fseek(fileEds,location,SEEK_SET); 
		rrn = (ftell(fileBooks)-61)/48;
		fwrite(name, 20, 1, fileEds);
		fwrite(&id,4,1,fileEds);
		fwrite(adress,20,1,fileEds);

		//escribo nueva pos dicponible en el archivo
		fseek(fileEds,0,SEEK_SET);
		fwrite(&value_avl,4,1,fileEds);

	}
	fileEds = fopen("Editoriales.bin","r+b");
	fseek(fileEds,5,SEEK_SET);
	fwrite(&dB,1,1,fileEds);
	fflush(fileEds);
	cout << "Editorial Succesfully Added! :D" << endl;	
	return rrn;
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
				cout << " " << isbn << " | " << name << " | " << author << " | ED" << id_editorial << endl;
			}
			current++;
		}
	}

	else{ //editorials

		fileEds = fopen("Editoriales.bin","rb");
		fseek(fileEds,0,SEEK_END);
		int number_registers = (ftell(fileEds)-52)/44;
		
		fseek(fileEds,52,SEEK_SET);
		char name[20];
		char adress[20];
		int id;

		int current = 0;
		while(current < number_registers){
			fread(name, 20, 1, fileEds);
			fread(&id, 4, 1, fileEds);
			fread(adress,20,1,fileEds);
			if(name[0] != '*'){
				cout << " " << id << " | " << name << " | " << adress << endl;
			}	
			current++;
		}
	}
	//dirtyBit = 1;
}
	
void RecordFile::readRecord(int rrnORaccess, int type){

	if(type == 1){ // read book
		fileBooks = fopen("Libros.bin","rb");
		int location = (rrnORaccess-1)*48 + 61;
		fseek(fileBooks,location,SEEK_SET);
		char name[20];
		char author[20];
		int isbn;
		int id_editorial;
		fread(name,20,1,fileBooks);

		if(name[0] != '*'){		

			fread(&isbn,4,1,fileBooks);
			fread(author,20,1,fileBooks);
			fread(&id_editorial,4,1,fileBooks);
			cout << isbn << " | " << name << " | " << author << " | " << id_editorial << endl;
		}

		else{

			bool found = false;
			while(found == false){
				location += 48;
				fseek(fileBooks,location,SEEK_SET); //me muevo al siguiente record
				fread(name,20,1,fileBooks);
				if(name[0] != '*')
					found = true;
			}
			fread(&isbn,4,1,fileBooks);
			fread(author,20,1,fileBooks);
			fread(&id_editorial,4,1,fileBooks);
			cout << isbn << " | " << name << " | " << author << " | " << id_editorial << endl;
		}		
	}

	else{ // access editorial

		fseek(fileBooks,61,SEEK_END);
		int number_registers = ftell(fileBooks)/48;

		fileBooks = fopen("Libros.bin","rb");
		fseek(fileBooks,61,SEEK_SET);
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
				cout  << isbn << " | " << name << " | " << author << " | " << id_editorial << endl;
				found = true;
			}
			current++;
		}

		if(!found){
			cout << "Sorry, there are no books in this editorial or this editorial doesn't esxist";
		}
	}	
}

int RecordFile::deleteRecord(int rrn, int type){
	bool dB = 1;
	int id;
	if(type == 1){
		
		int location = 61 + (rrn-1)*48;
		fileBooks = fopen("Libros.bin","r+b");
		int number_registers = ftell(fileBooks)/48;
		fseek(fileBooks,location,SEEK_SET);
		cout << ftell(fileBooks);
		char mark[1] = {'*'};
		fwrite(mark,1,1,fileBooks);

		fseek(fileBooks,location+20,SEEK_SET);
		fread(&id,4,1,fileBooks);

		int avl;
		fseek(fileBooks,0,SEEK_SET);
		fread(&avl,4,1,fileBooks); //primer elemento availList 
		fseek(fileBooks,0,SEEK_SET); //ya estoy en el primer byte
		fwrite(&rrn,4,1,fileBooks); 
		fseek(fileBooks,location+1,SEEK_SET);
		fwrite(&avl,4,1,fileBooks); 
		cout << "Book Succesfully Removed! :D";

		fseek(fileBooks,5,SEEK_SET);
		fwrite(&dB,1,1,fileBooks);
		fflush(fileBooks);
	}

	else if(type == 2){
		int location = 52 + (rrn-1)*44;
		fileEds= fopen("Editoriales.bin","r+b");
		int number_registers = ftell(fileEds)/44;
		fseek(fileEds,location,SEEK_SET);
		cout << ftell(fileEds);
		char mark[1] = {'*'};
		fwrite(mark,1,1,fileEds);

		fseek(fileBooks,location+20,SEEK_SET);
		fread(&id,4,1,fileBooks);

		int avl;
		fseek(fileEds,0,SEEK_SET);
		fread(&avl,4,1,fileEds); //primer elemento availList 
		fseek(fileEds,0,SEEK_SET); //ya estoy en el primer byte
		fwrite(&rrn,4,1,fileEds); 
		fseek(fileEds,location+1,SEEK_SET);
		fwrite(&avl,4,1,fileBooks); 
		cout << "Editorial Succesfully Removed! :D";

		fseek(fileEds,5,SEEK_SET);
		fwrite(&dB,1,1,fileEds);
		fflush(fileEds);

	}

	return id;
	
}

bool RecordFile::fileBooksExists(){
	ifstream ifile("Libros.bin");
	return ifile;
}

bool RecordFile::fileEdsExists(){
	ifstream ifile("Editoriales.bin");
	return ifile;
}

void RecordFile::updateRecord(int rrn, int type){

	bool modify = true;
	int field;
	int location;
	bool dB = 1;
	if(type == 1){

		location = 61 + (rrn-1)*48;
		fileBooks = fopen("Libros.bin","r+b");

		while(modify){
			cout << "\n   Update: \n  1. Name \n  2. ISBN \n  3. Author  \n  4. ID Editorial  \nChoose field: ";
			cin >> field;

			if(field == 1){ //update name
				fseek(fileBooks,location,SEEK_SET);
				char newName[20];
				cout << "New name: ";
				cin >> newName;
				fwrite(newName,20,1,fileBooks);
			}

			else if(field == 2){ //update ISBN (LLAVE PRIMARIA)
				fseek(fileBooks,location+20,SEEK_SET);
				int newISBN;
				cout << "New ISBN: ";
				cin >> newISBN;
				fwrite(&newISBN,4,1,fileBooks);
			}

			else if(field == 3){ //update author
				fseek(fileBooks,location+24,SEEK_SET);
				char newAuthor[20];
				cout << "New author: ";
				cin >> newAuthor;
				fwrite(newAuthor,20,1,fileBooks);
			}

			else if(field == 4){ //update editorial id
				fseek(fileBooks,location+44,SEEK_SET);
				int newED;
				cout << "New ED:  ";
				cin >> newED;
				fwrite(&newED,4,1,fileBooks);
			}
			else{
				cout << "Invalid option!";
			}

			int option;
			cout << "Update another field? [1. YES/ 2. NO]: ";
			cin >> option;

			if(option == 2){
				modify = false;
				fflush(fileBooks);
			}

		}

		fseek(fileBooks,5,SEEK_SET);
		fwrite(&dB,1,1,fileBooks);
		cout << "Book Succesfully Updated ! :D";
	}

	else if(type == 2){

		location = 51 + (rrn-1)*44;
		fileEds = fopen("Editoriales.bin","r+b");

		while(modify){
			cout << "\n   Update: \n  1. Name \n  2. ID \n  3. Adress \nChoose field: ";
			cin >> field;

			if(field == 1){ //update name
				fseek(fileEds,location,SEEK_SET);
				char newName[20];
				cout << "New name: ";
				cin >> newName;
				fwrite(newName,20,1,fileEds);
			}

			else if(field == 2){ //update ID (LLAVE PRIMARIA)
				fseek(fileEds,location+20,SEEK_SET);
				int newID;
				cout << "New ID: ";
				cin >> newID;
				fwrite(&newID,4,1,fileEds);
			}

			else if(field == 3){ //update adress
				fseek(fileEds,location+24,SEEK_SET);
				char newAdress[20];
				cout << "New adress: ";
				cin >> newAdress;
				fwrite(newAdress,20,1,fileEds);
			}

			else{
				cout << "Invalid option!";
			}

			int option;
			cout << "Update another field? [1. YES/ 2. NO]: ";
			cin >> option;

			if(option == 2){
				modify = false;
				fflush(fileEds);
			}

		}

		fseek(fileEds,5,SEEK_SET);
		fwrite(&dB,1,1,fileEds);
		cout << "Editorial Succesfully Updated ! :D";
	}
}

void RecordFile::crossList(){
	fileBooks = fopen("Libros.bin","rb");
	fileEds = fopen("Editoriales.bin","rb");

	int locB = 61;
	int locE = 52;
	fseek(fileBooks,0,SEEK_END);
	int num_books = (ftell(fileBooks)-61)/48;
	fseek(fileEds,0,SEEK_END);
	int num_eds = (ftell(fileEds)-52)/44;

	//Eds
	char ed[20];
	int id_ed;
	char adress[20];
	fseek(fileEds,52,SEEK_SET);

	//Books
	char book[20];
	int isbn;
	char author[20];
	int bID_ed;
	fseek(fileBooks,61,SEEK_SET);

	bool contains_books = false;
	for(int  i = 0; i < num_eds; i++){
		fread(ed,20,1,fileEds);
		fread(&id_ed,4,1,fileEds);
		fread(adress,20,1,fileEds);

		if(ed[0] != '*'){
			cout << " ED" << id_ed << " | " << ed << " | " << adress << endl;
			for(int j = 0; j < num_books; j++){
				fread(book,20,1,fileBooks);
				fread(&isbn,4,1,fileBooks);
				fread(author,20,1,fileBooks);
				fread(&bID_ed,4,1,fileBooks);

				if(book[0] != '*'){
					if(id_ed == bID_ed){
						cout << "\t" << isbn << " | " << book << " | " << author << endl; 
						contains_books = true;
					}
				}
			}
			fseek(fileBooks,61,SEEK_SET);
		}
		if(contains_books == false)
			cout << "NO BOOKS IN THIS EDITORIAL";
	}

}

bool RecordFile::dirtyBitBooks(){
	bool dB;
	fileBooks = fopen("Libros.bin","rb");
	fseek(fileBooks,5,SEEK_SET);
	fread(&dB,1,1,fileBooks);
	return dB;
}

bool RecordFile::dirtyBitEds(){
	bool dB;
	fileEds = fopen("Editoriales.bin","rb");
	fseek(fileEds,5,SEEK_SET);
	fread(&dB,1,1,fileEds);
	return dB;
}

bool RecordFile::indexBooksExists(){
	ifstream ifile("IndexLibros.bin");
	return ifile;
}

bool RecordFile::indexEdsExists(){
	ifstream ifile("IndexEditoriales.bin");
	return ifile;
}

FILE* RecordFile::getIndexBooks(){
	return indexBooks;
}

FILE* RecordFile::getIndexEds(){
	return indexEds;
}

void RecordFile::printIndex(int type){

	if(type == 1){
		indexBooks = fopen("IndexLibros.bin","rb");
		fseek(indexBooks,0,SEEK_END);
		int num_books = (ftell(indexBooks)-17)/8;
		fseek(indexBooks,17,SEEK_SET);
		int key, rrn;

		for(int i = 0; i < num_books; i++){
			fread(&key,4,1,indexBooks);
			fread(&rrn,4,1,indexBooks);
			cout << " " << key << " | " << rrn << endl;
		}
	}

	else{
		indexEds = fopen("IndexEditoriales.bin","rb");
		fseek(indexEds,0,SEEK_END);
		int num_eds = (ftell(indexEds)-17)/8;
		fseek(indexEds,17,SEEK_SET);
		int key, rrn;

		for(int i = 0; i < num_eds; i++){
			fread(&key,4,1,indexEds);
			fread(&rrn,4,1,indexEds);
			cout << " " << key << " | " << rrn << endl;
		}
	}
}




