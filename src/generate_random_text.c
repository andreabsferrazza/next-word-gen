#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

struct ptable_info{
	int max_word_length;
	int total_words;
};

struct ptable_info scan_dictionary(const char* filename){
	int c;
	int n=0;
	int word_identifier=0;
	int max_word_length = 6; // a probability may have six digits 
	int total_words = 0;
	struct ptable_info pti;
	FILE *file;
	if ((file=fopen(filename,"r"))==NULL) {
		printf("Wrong input file name\n");
		exit(1);
	}
	do{
		c=fgetc(file);
		// if we already encountered a character (n==0) that is not the EOF, '\n' or ',' 
		// and the current character IS the EOF, '\n' or ','
		if(n>0 && (c==',' || c=='\n' || c==EOF)){
			// we have found a word
			if(word_identifier==0){
				if(n>max_word_length ){
					max_word_length=n;
				}
				total_words++;
			}
			if(c=='\n'){
				word_identifier=0;
			}else{
				word_identifier++;
			}
			n=0;
			// printf("%d\n\n",word_identifier);
			continue;
		}
		// if the current character is a valid one
		if( (c>='a' && c<='z') || (c>='0' && c<='9') || c=='\'' || c=='?' || c=='!' || c=='.'){
			// number of letters of the currend word
			n++;
		}
	}while (c != EOF);
	fclose(file);
	pti.total_words = total_words;
	pti.max_word_length = max_word_length;
	return pti;
}

int copy_string(char* src, char* dest){
	int z=0;
	do{
		dest[z]=src[z];
		z++;
	}while(src[z]!='\0');
	dest[z++]='\0';
	return 0;
}
int generate_random_text(const char* filename, int words_number, const char * first_word){
	// ###################### 1. we scan the file to determine the max_word_length and the total_words in the dictionary
	struct ptable_info pti; 
	pti = scan_dictionary(filename);
	int max_word_length = pti.max_word_length;
	int total_words = pti.total_words;

	if(max_word_length==0 || total_words==0){
		exit(1);
	}
	printf("mwl: %d | tw: %d\n",max_word_length,total_words);

	// ###################### 2. we parse the csv
	int c;
	int words=0,n=0;
	int word_identifier=0;
	char dictionary[total_words][max_word_length+1];
	char next_words[total_words][total_words+1][max_word_length+1];
	char next_words_probability[total_words][total_words][6];
	int next_words_counter[total_words];
	initialize_to(next_words_counter,total_words,0);
	FILE *file;
	if ((file=fopen(filename,"r"))==NULL) {
		printf("Wrong input file name\n");
		exit(1);
	}
	do{
		c=fgetc(file);
		// if we already encountered a character (n==0) that is not the EOF, '\n' or ',' 
		// and the current character IS the EOF, '\n' or ','
		if(n>0 && (c==',' || c=='\n' || c==EOF)){
			// we have found a word
			if(word_identifier==0){
				dictionary[words][n]='\0';
				printf("%s\n",dictionary[words]);
			}
			if(word_identifier>0){
				// in the odd positions between commas there are the
				// next words of "word"
				if(word_identifier%2==1){
					next_words[words][ next_words_counter[words] ][n]='\0';
					// printf("%d %s parola ###\n ",word_identifier,next_words[words][ next_words_counter[words] ]);
				}else{
				// ... and in the even positions the probability
					next_words_probability[words][ next_words_counter[words] ][n]='\0';
					// printf("%s |",next_words_probability[words][ next_words_counter[words] ]);
					next_words_counter[words]++;
				}
			}
			if(c=='\n'){
				word_identifier=0;
				words++;
				printf("\n");
			}else{
				word_identifier++;
			}
			// printf("%d\n\n",word_identifier);
			n=0;
			continue;
		}
		// if the current character is a valid one
		if( (c>='a' && c<='z') || (c>='0' && c<='9') || c=='\'' || c=='?' || c=='!' || c=='.'){
			if(word_identifier==0){
				dictionary[words][n] = c;
			}
			if(word_identifier>0){
				// in the odd position between commas there are the
				// next words of "word"
				if(word_identifier%2==1){
					next_words[words][ next_words_counter[words] ][n]=c;
				}else{
				// ... and in the even positions the probability
					next_words_probability[words][ next_words_counter[words] ][n]=c;
				}
			}
			// number of letters of the currend word
			n++;
		}
	}while (c != EOF);
	fclose(file);
	for(int i=0;i<total_words;i++){
		printf("%s ",dictionary[i]);
		for(int j=0;j<next_words_counter[i];j++){
			printf(", %s = %s",next_words[i][j],next_words_probability[i][j]);
		}
		printf("\n");
	}
	return 0;
}
