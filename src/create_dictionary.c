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
	// printf("%s\n",filename);
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
			// printf("%d\n",c);
		// se abbiamo già incontrato un carattere che non sia EOF o uno spazio
		// AND il carattere corrente è EOF o uno spazio
		if(n>0 && (c==' ' || c==EOF || c=='\n')){
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
		/* if(c==195){
			n++;
		}
		if(c==160  || c==168 || c==169 || c==172 || c==178 || c==185 ||
			c==128 || c==136 || c==137 || c==140 || c==146 || c==153){
			n++;
		} */
		// se il carattere corrente c è tra quelli che possono comporre una parola
		if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || c=='\'' || c>160){
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
		printf("tw %d - mwl %d\n",total_words,max_word_length);
		// array for the words in the text 
		// we add +1 to the max_word_length because we use '\0' as string's last character
		char dictionary[total_words][max_word_length+1];
		// int next_words[total_words][10][2];
		int ** next_words;
		next_words = malloc( sizeof(int*) * total_words);
		for(int i=0;i<total_words;i++){
			// we chose to alloc 10 elements so we limit the number of realloc
			next_words[i] = malloc( sizeof(int) * 10);
		}
		int ** frequence;
		frequence = malloc( sizeof(int*) * total_words);
		for(int i=0;i<total_words;i++){
			// we chose to alloc 10 elements so we limit the number of realloc
			frequence[i] = calloc(0, sizeof(int) * 10);
		}
		// int next_words_unique_count[total_words];
		int * next_words_unique_count;
		next_words_unique_count = calloc(total_words, sizeof(int));
		int * next_words_total_count;
		next_words_total_count = calloc(total_words, sizeof(int));
		char * current_word = NULL;
		current_word = (char*) malloc(2*(max_word_length+1)*sizeof(char));
		// words is the counter for the words, n for the letters in a word
		int words=0, n=0;
		int last_word_index=0;
		do{
			// we scan each character of the file until the end of the file (EOF)
			c=fgetc(file_catcher);
			// printf("%d\n",c);
			// if n>0 means that we have already begun to write a word
			// so we terminate it with \0, we set n=0 for the next word and words++
			// and we skip to the next iteration of the while
			if(n>0 && (c==' ' || c=='\n' || c==EOF || c=='?' || c=='!' || c=='.')){
				// if we encounter ? ! or . we must store it as a separate word, so we repeat the 
				// block responsible for storing the word
				int repeat = 1;
				if(c=='?' || c=='!' || c=='.'){
					repeat = 2;
				}
				for(int r=0;r<repeat;r++){
					int current_index = words;
					// we need to take in the current_word ! ? or .
					if(r>0){
						current_word[n++] = c;
					}
					// we close the word with the terminator \0
					current_word[n++] = '\0';

					// we determine if the world is already in the dictionary
					int check=0;
					for(int w=0;w<words;w++){
						// printf("%s\n",current_word);
						check = compare_words(dictionary[w], current_word);
						if(check>0){
							// we found the word, we take the index
							current_index = w;
							break;
						}
					}
					// the word is not in the dictionary, so we store it
					if(check==0){
						strcpy(dictionary[words], current_word);
						// printf("%s\n",dictionary[words]);
						/* free(current_word);
						current_word = (char*) malloc((max_word_length+1)*sizeof(char)); */
						// printf("%s\n", dictionary[words]);
						words++;
					}
					// we reset the character counter
					n=0;
					// we now store the current_word as successor of the previous
					// we use the index of the dictionary as reference
					if(words>1){ // we cannot set the first word as next of the last now (because we don't know the last)
						// TODO
						// we check if it is already a next_word of the last_word
						// if yes we have to update its frequence
						// if no we must store it and update its frequence
						
						int check=0;
						int current_nw_index=next_words_unique_count[ last_word_index ];
						for(int nw=0;nw<next_words_unique_count[ last_word_index ];nw++){
							if(next_words[ last_word_index ][nw]==current_index){
								check++;
							}
							if(check>0){
								// we found the word, we take the index
								current_nw_index = nw;
								break;
							}

						}
						if(check==0){
						// if it is not the first next_word of the last_word we need more space
							if(sizeof(next_words[ last_word_index ]) <= next_words_unique_count[ last_word_index ]){
								int next_size =  next_words_unique_count[ last_word_index ] + 1;
								int * new_next_word = realloc(next_words[ last_word_index ], sizeof(int) * next_size);
								// int * new_frequence = realloc(frequence[ last_word_index ], sizeof(int) * next_size);
								// printf("realloc next words\n");
								if(new_next_word==NULL){
								// if(new_next_word==NULL || new_frequence==NULL){
									printf("Realloc failed\n");
									for(int i=0;i<total_words;i++){
										free(next_words[i]);
									}
									free(next_words);
									for(int i=0;i<total_words;i++){
										free(frequence[i]);
									}
									free(frequence);
									free(current_word);
									free(next_words_unique_count);
									exit(1);
								}
								next_words[ last_word_index ] = new_next_word;
								// frequence[ last_word_index ] = new_frequence;
							}
							next_words[ last_word_index ][ next_words_unique_count[ last_word_index ] ] = current_index;
							// frequence[ last_word_index ][ next_words_unique_count[ last_word_index ] ] = 0;
							next_words_unique_count[ last_word_index ]++;
						}
						// frequence[ last_word_index ][ current_nw_index ] ++;
					}
					last_word_index = current_index;
					// printf("%d - %s\n",current_index,dictionary[current_index]);
				}
				continue;
			}
			// 160,168,169,172,178,185,128,136,137,140,146,153
			if(c==195){
				// printf("%d n=%d\n",c,n);
				current_word[n] = (char) c;
				n++;
				c=fgetc(file_catcher);

				if(c==160  || c==168 || c==169 || c==172 || c==178 || c==185 ||
       					c==128 || c==136 || c==137 || c==140 || c==146 || c==153){
					// printf("%d ifnext=%d\n",c,n);
					current_word[n] = (char) c;
					n++;
					continue;
				}
			}
			// if c is a "valid" character we put it in the dictionary
			if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || c=='\''){
				// printf("%c - %d\n",c,c);
				
				// we consider all the upper case letters as lowercase
				if(c>='A' && c<='Z'){
					c+=32;
				}
				current_word[n] = (char) c;
				n++;
			}
		}while (c != EOF);
		// we must set here the first word as the next of the last
		next_words[words-1][0] = 0;
		next_words_unique_count[words-1]++;

		// #### Print
		printf("Inizio stampa del dizionario\n");
		for(int w=0; w<words; w++){
			printf("%s ===> ", dictionary[w]);
			for(int j=0;j<next_words_unique_count[w];j++){
				printf(" %d ",next_words[w][j]);
			}
			printf("\n");
		}
		// ##### We free the memory
		for(int i=0;i<total_words;i++){
			free(next_words[i]);
		}
		free(next_words);
		for(int i=0;i<total_words;i++){
			free(frequence[i]);
		}
		free(frequence);
		free(current_word);
		free(next_words_unique_count);
		fclose(file_catcher);

		printf("%d uniques\n",words);
		exit(1);
		// ############ 4. Compute frequences
		// we need to have unique next words
		int u = words;
      		int nw_unique[total_words][total_words+1];
		// so we can have for each next word its frequence
		int nw_frequence[total_words][total_words+1];
		// counter for the unique next words
		int nw_unique_count[total_words];
		initialize_to(nw_unique_count,total_words,0);
		
		// for each unique word
		for(int i=0;i<u;i++){
			// for each next word of the unique word i
			for(int j=0;j<next_words_unique_count[i];j++){
				// nw_frequence[i][j]=1;
				// for each unique next word
				int new_next_word=1;
				for(int k=0;k<nw_unique_count[i];k++){
					if(k!=j && nw_unique[i][k]==next_words[i][j]){
						new_next_word=0;
						nw_frequence[i][k]++; 
					}
				}
				if(new_next_word==1){
					nw_unique[i][ nw_unique_count[i] ] = next_words[i][j];
					nw_frequence[i][ nw_unique_count[i] ] = 1;
					nw_unique_count[i]++;
				}
			}
		}

		char* filename_out = "output.txt";
		FILE *fpout;
		if ((fpout=fopen(filename_out,"wt"))==NULL) {
			printf("Wrong output file name");
			exit(1);
		}

		for(int i=0;i<u;i++){
			fprintf(fpout,"%s",dictionary[i]);
			for(int a=0;a<nw_unique_count[i];a++){
				float perc=(float) nw_frequence[i][a]/ (float) next_words_unique_count[i];
				fprintf(fpout,",%s,%.4g",dictionary[ nw_unique[i][a] ],perc);
			}
			fprintf(fpout,"\n");
		}
		fclose(fpout);
		printf("Found %d words | %d uniques | %d maxlength of a word.\n",total_words,u,max_word_length);
	}
	return 0;
}

