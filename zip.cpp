#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>

using namespace std;

unsigned char* convertCodeWord(unsigned int code1, unsigned int code2){
	unsigned char* out = new unsigned char[3];
	
	out[0] = code1 >> 4;
	out[1] = ((code1 & 0xf) << 4) | (code2 >> 8);
	out[2] = code2 & 0xff;

	return out;
}

int main(int argc, char** argv){
	if(argc!=2){
		printf("USAGE: zip filename\n");
		exit(0);
	}

	FILE* file;
	FILE* outfile;

	string out = argv[1];	
	out = out + ".zip";	

	if((file = fopen(argv[1], "r"))==NULL){
		printf("File could not be opened\n");
		return -1;
	}
	if((outfile=fopen(out.c_str(), "w"))==NULL){
		printf("Could not open zip file\n");
		return -1;
	}

	unsigned int MAX_SIZE=4096;
	
	map<string,unsigned int> dictionary;

	for(unsigned int i = 0; i<256; i++){
		dictionary[string(1,(unsigned char) i)] = i;
	}
	
	string currentString = string(1,fgetc(file));
	unsigned int previousCodeword;
	char currentChar;
	int isSecondIter = 0;

	while((currentChar = fgetc(file))!=EOF){
		if(ferror(file)){
			printf("Error reading file\n");
			return -2;
		}
		string s = currentString+currentChar;
		if(dictionary.find(s)!=dictionary.end()){
			currentString = s;
		}
		else{
			unsigned int codeword = dictionary[currentString];
			//If we have a codeword to combine with, do that
			if(isSecondIter==1){
				unsigned char* outs = convertCodeWord(previousCodeword, codeword);
				if(fwrite(outs,sizeof(unsigned char), 3, outfile)!=3){
					printf("Error writing to file\n");
					return -3;
				}
				isSecondIter=0;
			}
			//elsewise save the current codeword to combine with the next
			else{
				previousCodeword = codeword;
				isSecondIter=1;
			}
			if(dictionary.size()<MAX_SIZE-1){
				dictionary[s] = dictionary.size()-1;
			}
			currentString = string(1,currentChar);
		}
	}	
	unsigned int codeword = dictionary[currentString];
	if(isSecondIter==0){
		unsigned char* outs = convertCodeWord(codeword, 4095);
		if(fwrite(outs,sizeof(unsigned char), 3, outfile)!=3){
			printf("Error writing to file\n");
			return -3;
		}
        }
	else{
		unsigned char* outs = convertCodeWord(previousCodeword, codeword);
		if((fwrite(outs,sizeof(unsigned char), 3, outfile))!=3){
			printf("Error writiing to file\n");
			return -3;
		}
	}
	
	fclose(file);
	fclose(outfile);
	
	return 1;
}
