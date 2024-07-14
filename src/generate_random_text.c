#include <stdio.h>
#include <stdlib.h>

struct ptable_info{
	int max_word_length;
	int total_words;
};

int generate_random_text(const char* fn, int nw, const char * fw){
	printf("%s\n",fn);
	printf("%d\n",nw);
	printf("%s\n",fw);
	// we start parsing the input file
	FILE *file;
	// check if fn is ok
	printf("%s\n",fn);
	if ((file=fopen(fn,"r"))==NULL) {
		printf("Wrong input file name\n");
		exit(1);
	}
	// unique words
	// probability
	// next word counter
		
	int max_word_length = 0;
	int total_words = 0;
	// struct ptable_info di; 
	int c;
	int n=0;
	int word_identifier=0;
	do{
		c=fgetc(file);
		// se abbiamo già incontrato un carattere che non sia EOF o uno spazio
		// AND il carattere corrente è EOF o uno spazio
		printf("%c",c);
		if(n>0 && (c==',' || c=='\n' || c==EOF)){
			// abbiamo quindi trovato una parola
			if(word_identifier==0){
				// Calcolo massima lunghezza di una parola
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
		// se il carattere corrente c è tra quelli che possono comporre una parola
		if( (c>='a' && c<='z') || (c>='0' && c<='9') || c=='\'' || c=='?' || c=='!' || c=='.'){
			// contiamo con il contatore n il numero di lettere della parola che stiamo leggendo
			n++;
		}
	}while (c != EOF);
	fclose(file);
	/* di.max_word_length = max_word_length;
	di.total_words= total_words; */
	printf("mwl: %d | tw: %d\n",max_word_length,total_words);
	return 0;
}
