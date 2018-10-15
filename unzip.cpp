#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>

using namespace std;

unsigned int code1;
unsigned int code2;
char currentChar;
string currentString;
string oldString;
unsigned int currentCode;
unsigned int nextCode;
map<unsigned int, string> dictionary;
FILE* file;
FILE* outfile;
unsigned int MAX_SIZE=4096;

void getCodes(unsigned char a, unsigned char b, unsigned char c){
	code1 = (a << 4) | (b >> 4);
	code2 = ((b&0xf)<<8) | (c);
}

void uncompress(){
	if(dictionary.find(nextCode)==dictionary.end()){
                currentString = dictionary[currentCode];
                currentString = currentString + currentChar;
        }
        else{
                currentString = dictionary[nextCode];
        }
	
	//cout << currentString << "\n";
        fwrite(currentString.c_str(), sizeof(unsigned char), currentString.size(), outfile);
        currentChar = currentString[0];
        oldString = dictionary[currentCode];
	if(dictionary.size()<MAX_SIZE-1){
        	int size = dictionary.size();
		dictionary[size] = oldString + currentChar;
	}
        currentCode = nextCode;
}

int main(int argc, char** argv){
	if(argc!=2){
		printf("USAGE: zip filename\n");
		exit(0);
	}

	string out = argv[1];
	string zip = out;
	int index = out.size()-4;	
	out = out.substr(0, index);	
	zip = zip.substr(index);

	if((file = fopen(argv[1], "r"))==NULL){
		printf("Zip file could not be opened\n");
		return -1;
	}
	if((outfile=fopen(out.c_str(), "w"))==NULL){
		printf("Could not open out file\n");
		return -1;
	}
	if(zip != ".zip"){
		printf("USAGE: zip filename.zip, file must be a zip file\n");
		exit(0);
	}

	for(unsigned int i = 0; i<256; i++){
		dictionary[i] = string(1,(unsigned char) i);
	}

	unsigned char a;
	unsigned char b;
	unsigned char c;

	int numRead = 0;

	a = fgetc(file);
	b = fgetc(file);
	c = fgetc(file);
	
	getCodes(a,b,c);
	
	currentCode = code1;
	nextCode = code2;

	string s = dictionary[currentCode];
	currentChar = s[0];
	fputc(currentChar, outfile);

	uncompress();

	int read;
	while((read = fgetc(file))!=EOF){
		if(numRead == 0){
			numRead++;
			a = read;
		}
		else if(numRead == 1){
			numRead++;
			b = read;
		}
		else{
			numRead = 0;
			c = read;
			getCodes(a,b,c);
			
			nextCode = code1;
			uncompress();
			nextCode = code2;
			uncompress();
		}
	}
	
	return 1;
}
