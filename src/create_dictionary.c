#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

struct dictionary_info{
	int max_word_length;
	int total_words;
};

struct dictionary_info scan_words(const char * filename){
	struct dictionary_info di;
	int c;
	int n=0;
	FILE *file;
	// check if filename is ok
	printf("%s\n",filename);
	if ((file=fopen(filename,"r"))==NULL) {
		printf("Wrong input file name\n");
		exit(1);
	}
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
		if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || c=='\''){
			// contiamo con il contatore n il numero di lettere della parola che stiamo leggendo
			n++;
		}
	}while (c != EOF);
	di.max_word_length = max_word_length;
	di.total_words= total_words;
	fclose(file);
	return di;
}

int create_dictionary(const char * filename){
	/*	Each phrase is terminated by . ? ! or EOF 
	 *	. ? and ! are words
	 *	every other character sequence between spaces and/or . ? ! is a word
	 *	the last word of the file has the first as "next"
	 *	all other characters are ignored except for a-zA-Z'
	 * */

	struct dictionary_info di;
	int c;
	di = scan_words(filename);

	if(di.total_words>0 && di.max_word_length>0){
		
		FILE *file_catcher = fopen(filename, "r");
		int total_words = di.total_words; // computed number of words
		int max_word_length = di.max_word_length; // max length possible of a word
		// array for the words in the text 
		// we add +1 to the max_word_length because we use '\0' as string's last character
		// char words_array[total_words][max_word_length+1];
		char ** words_array;
		words_array = malloc( sizeof(char*) * total_words);
		for(int i=0;i<total_words;i++){
			words_array[i] = malloc( sizeof(char) * (max_word_length+1) );
		}
		// int next_words[total_words][10][2];
		int ** next_words;
		next_words = malloc( sizeof(int*) * total_words);
		// int next_words_count[total_words];
		int * next_words_count;
		next_words_count = calloc(total_words, sizeof(int));
		for(int i=0;i<total_words;i++){
			next_words[i] = malloc( sizeof(int) );
		}
		char * current_word = NULL;
		current_word = (char*) malloc((max_word_length+1)*sizeof(char));
		// words is the counter for the words, n for the letters in a word
		int words=0, n=0;
		int last_word_index=0;
		do{
			// we scan each character of the file until the end of the file (EOF)
			c=fgetc(file_catcher);
			// if n>0 means that we have already begun to write a word
			// so we terminate it with \0, we set n=0 for the next word and words++
			// and we skip to the next iteration of the while
			if(n>0 && (c==' ' || c==EOF || c=='?' || c=='!' || c=='.')){
				int repeat = 1;
				if(c=='?' || c=='!' || c=='.'){
					repeat = 2;
				}
				for(int r=0;r<repeat;r++){
					int current_index = words;
					if(r>0){
						current_word[n++] = c;
						current_word[n++] = '\0';
					}
					current_word[n++] = '\0';
					int check=0;
					for(int w=0;w<words;w++){
						check = compare_words(words_array[w],current_word);
						if(check>0){
							// we found the word, we take the index
							current_index = w;
							break;
						}
					}
					if(check==0){
						strcpy(words_array[words], current_word);
						free(current_word);
						current_word = (char*) malloc((max_word_length+1)*sizeof(char));
						// printf("%s\n", words_array[words]);
						words++;
					}
					n=0;
					// qui dobbiamo associare la parola che abbiamo trovato alla sua precedente
					// utilizzando gli indici
					if(last_word_index>0){
						if(sizeof(next_words[ last_word_index ]) > next_words_count[ last_word_index ]){
							next_words[ last_word_index ][ next_words_count[ last_word_index ] ]= current_index;
							next_words_count[ last_word_index ]++;
						}else{
							// we need more space
							int * res = realloc(next_words[ last_word_index ], sizeof(int) * (next_words_count[ last_word_index ] + 1));
							if(res==NULL){
								printf("Realloc failed\n");
								for(int i=0;i<total_words;i++){
									free(next_words[i]);
								}
								for(int i=0;i<total_words;i++){
									free(words_array[i]);
								}
								free(words_array);
								free(current_word);
								free(next_words);
								free(next_words_count);
								exit(1);
							}
							next_words[ last_word_index ] = res;
							next_words[ last_word_index ][ next_words_count[ last_word_index ] ] = current_index;
							next_words_count[ last_word_index ]++;
						}
					}
					last_word_index = current_index;
					// printf("%d - %s\n",current_index,words_array[current_index]);
				}
				continue;
			}
			// if c is a "valid" character we put it in the words_array
			if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || c=='\''){
				// we consider all the upper case letters as lowercase
				if(c>='A' && c<='Z'){
					c+=32;
				}
				current_word[n++] = (char) c;
				// words_array[words][n++] = (char) c;
			}
		}while (c != EOF);
		/* printf("%d vs %d\n",words,total_words);
		exit(0); */
		if(words<total_words){
			// we need less space
			/* char ** res = realloc( words_array, sizeof(char) * words);
			if(res==NULL){
				printf("Realloc failed\n");
				for(int i=0;i<total_words;i++){
					free(next_words[i]);
				}
				for(int i=0;i<total_words;i++){
					free(words_array[i]);
				}
				free(words_array);
				free(current_word);
				free(next_words);
				free(next_words_count);
				exit(1);
			}
			words_array = res; */
		}
		for(int w=0; w<total_words; w++){
			printf("%s ===> ",words_array[w]);
			for(int j=0;j<next_words_count[w];j++){
				printf(" %d ",next_words[w][j]);
			}
			printf("\n");
		}
		for(int i=0;i<total_words;i++){
			free(next_words[i]);
		}
		for(int i=0;i<total_words;i++){
			free(words_array[i]);
		}
		free(words_array);
		free(current_word);
		free(next_words);
		free(next_words_count);
		fclose(file_catcher);

		printf("%d uniques\n",words);
		printf("End on line 154\n");
		exit(1);
		// ############ 2. Identifying all unique words 
		// each unique word in the text
		char words_array_unique[total_words][max_word_length+1];
		// counter for the unique words
		int u=0;
		// foreach word in the text
		for(int i=0;i<words;i++){
			// printf("%s - %d\n",words_array[i],i);
			// flag to check if the word is already present in words_array_unique
			int already_seen_word=0;
			// foreach previous word in the text 
			for(int j=0;j<i;j++){
				// printf("i = %d | j = %d\n",i,j);
				already_seen_word = compare_words(words_array[i],words_array[j]);
				// if already_seen_word>0 we found that word i = word j
				if(already_seen_word>0){
					// printf("%s (i) %s (j)\n",words_array[i],words_array[j]);
					// already_seen_word==0 means different
					break;
				}
			}

			// if write_unique_word is 0 means that this is a unique word so far
			if(already_seen_word==0){
				int z=0;
				// so we copy it in the unique array
				do{
					words_array_unique[u][z]=words_array[i][z];
					z++;
				}while(words_array[i][z]!='\0');
				words_array_unique[u][z++]='\0';
				// printf("%s - u=%d\n",words_array_unique[u],u);
				// and increment the counter of the unique words
				u++;
			}
		}

		// ############ 3. Determining next words 
		// at this point words_array_unique will contain u words
		// nw will contain the indexes of the next words for the words contained in words_array_unique
		int nw[total_words][total_words+1];
		// counter for the next words
		int nw_count[total_words];
		initialize_to(nw_count,total_words,0);
		// for each unique word
		for(int i=0;i<u;i++){
			// for each word in the text
			for(int j=0;j<total_words;j++){
				// if the unique_word i = word j, we look for the next word
				int equal = compare_words(words_array_unique[i],words_array[j]);
				if(equal>0){
					// index for the next word in the text
					int next_index = j+1;
					if(next_index==total_words){
						// the last word has the first as "next"
						next_index=0;
					}
					// now we must find the correct index in the unique array
					// so we search the word j in the unique words
					for(int z=0;z<u;z++){
						int eq = compare_words(words_array_unique[z],words_array[next_index]);
						if(eq>0){
							next_index=z;
							break;
						}
					}
					// now we add the index to nw and adjust the counter
					nw[i][ nw_count[i] ] = next_index;
					nw_count[i]++;
				}
			}
		}

		// ############ 4. Compute frequences
		// we need to have unique next words
      		int nw_unique[total_words][total_words+1];
	printf("cane\n");exit(1);	
		// so we can have for each next word its frequence
		int nw_frequence[total_words][total_words+1];
		// counter for the unique next words
		int nw_unique_count[total_words];
		initialize_to(nw_unique_count,total_words,0);
		
		// for each unique word
		for(int i=0;i<u;i++){
			// for each next word of the unique word i
			for(int j=0;j<nw_count[i];j++){
				// nw_frequence[i][j]=1;
				// for each unique next word
				int new_next_word=1;
				for(int k=0;k<nw_unique_count[i];k++){
					if(k!=j && nw_unique[i][k]==nw[i][j]){
						new_next_word=0;
						nw_frequence[i][k]++; 
					}
				}
				if(new_next_word==1){
					nw_unique[i][ nw_unique_count[i] ] = nw[i][j];
					nw_frequence[i][ nw_unique_count[i] ] = 1;
					nw_unique_count[i]++;
				}
			}
			/* printf("Indice: %d - Parola: %s - Count: %d - Next: ",i,words_array_unique[i],nw_count[i]);
			for(int a=0;a<nw_unique_count[i];a++){
				printf("%s : %d,",words_array_unique[ nw_unique[i][a] ],nw_frequence[i][a]);
			}
			printf("\n"); */
		}

		char* filename_out = "output.txt";
		FILE *fpout;
		if ((fpout=fopen(filename_out,"wt"))==NULL) {
			printf("Wrong output file name");
			exit(1);
		}

		for(int i=0;i<u;i++){
			fprintf(fpout,"%s",words_array_unique[i]);
			for(int a=0;a<nw_unique_count[i];a++){
				float perc=(float) nw_frequence[i][a]/ (float) nw_count[i];
				fprintf(fpout,",%s,%.4g",words_array_unique[ nw_unique[i][a] ],perc);
			}
			fprintf(fpout,"\n");
		}
		fclose(fpout);
		printf("Found %d words | %d uniques | %d maxlength of a word.\n",total_words,u,max_word_length);
	}
	return 0;
}

