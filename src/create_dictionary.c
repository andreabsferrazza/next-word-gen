#include <stdio.h>
#include <stdlib.h>

struct dictionary_info{
	int max_word_length;
	int total_words;
};

struct dictionary_info scan_words(char * filename){
	struct dictionary_info di;
	int c;
	int n=0;
	FILE *file = fopen(filename, "r");
	int max_word_length = 0;
	int total_words = 0;
	if (file == NULL){
		di.max_word_length = max_word_length;
		di.total_words= total_words;
		return di; 
	}
	do{
		c=fgetc(file);
		// se abbiamo già incontrato un carattere che non sia EOF o uno spazio
		// AND il carattere corrente è EOF o uno spazio
		if(n>0 && (c==' ' || c==EOF)){
			// abbiamo quindi trovato una parola
			total_words++;
			// Calcolo massima lunghezza di una parola
			if(n>max_word_length){
				max_word_length=n;
			}
			n=0;
			continue;
		}
		// se il carattere corrente c è tra ?!. 
		if(c=='?' || c=='!' || c=='.'){
			// se n>0 abbiamo già trovato un carattere che fa parte di una parola
			// dobbiamo quindi registrare la parola in "sospeso" e poi memorizzare c
			if(n>0){
				total_words++;
				if(n>max_word_length){
					max_word_length=n;
				}
			}
			total_words++;
			if(n>max_word_length){
				max_word_length=n;
			}
			n=0;
			continue;
		}
		// se il carattere corrente c è tra quelli che possono comporre una parola
		if( (c>='a' && c<='z') || (c>='A' && c<='Z') || c=='\''){
			// contiamo con il contatore n il numero di lettere della parola che stiamo leggendo
			n++;
		}
	}while (c != EOF);
	di.max_word_length = max_word_length;
	di.total_words= total_words;
	return di;
}

int create_dictionary(char * filename){
	/*	Each phrase is terminated by . ? ! or EOF 
	 *	. ? and ! are words
	 *	every other character sequence between spaces and/or . ? ! is a word
	 *	the last word of the file has the first as "next"
	 *	all other characters are ignored except for a-zA-Z'
	 * */
	// TODO può contenere numeri?
	// Determines max_word_length and total_words

	struct dictionary_info di;
	int c;
	di = scan_words(filename);

	if(di.total_words>0 && di.max_word_length>0){
		int total_words = di.total_words;
		int max_word_length = di.max_word_length;
		printf("Found %d words | %d maxlength of a word.\n",total_words,max_word_length);
		// char* current_string;
		// current_string = malloc( max_word_length * sizeof(char) );
		// char* words_array[total_words+1];
		char words_array[total_words][max_word_length+1];
		char words_array_unique[total_words][max_word_length+1];
		int words = 0, n=0;
		// printf("%d\n",words);
		FILE *file_catcher = fopen(filename, "r");
		do{
			c=fgetc(file_catcher);
			if(n>0 && (c==' ' || c==EOF)){
				/* for(int i=0;i<n;i++){
					printf("%c\n",current_string[i]);
					words_array[words][i] = current_string[i];
					printf("%c\n",words_array[words][i]);
					printf("%s\n",words_array[words]);
				} */
				words_array[words][n++] = '\0';
				words++;
				n=0;
				continue;
			}
			if(c=='?' || c=='!' || c=='.'){
				if(n>0){
					/* for(int i=0;i<n;i++){
						words_array[words][i] = current_string[i];
					} */
					words_array[words][n++] = '\0';
					words++;
					n=0;
				}
				words_array[words][n++] = c;
				words_array[words][n++] = '\0';
				words++;
				n=0;
				continue;
			}
			if( (c>='a' && c<='z') || (c>='A' && c<='Z') || c=='\''){
				words_array[words][n++] = (char) c;
			}
		}while (c != EOF);

		// words_array
		int z,check;
		int u=0;
		for(int i=0;i<words;i++){
			// printf("%s - %d\n",words_array[i],i);
			check=0;
			for(int j=0;j<i;j++){
				z=0;
				while(words_array[i][z]!='\0' && words_array[j][z]!='\0'){
					if(words_array[i][z]!=words_array[j][z]){
						check=0;
						break;
					}
					// printf("%d : %c - %d : %c\n",i,words_array[i][z],j,words_array[j][z]);
					check++;
					z++;
				}
				if(check>0){
					break;
				}
			}
			if(check==0){
				z=0;
				u++;
				do{
					words_array_unique[u][z]=words_array[i][z];
					z++;
				}while(words_array[i][z]!='\0');
				words_array_unique[u][z++]='\0';
				printf("%s\n",words_array_unique[u]);
			}
		}
		fclose(file_catcher);
	}
	return 0;
}

