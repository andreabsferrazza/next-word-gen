#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
		if(n>0 && (c==' ' || c==EOF || c=='\n' || c=='\'')){
			if(c=='\''){
				n++;
			}
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
		if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || //c=='\'' ||
			c==195 || c==160  || c==168 || c==169 || c==172 || c==178 || c==185 ||
			c==128 || c==136 || c==137 || c==140 || c==146 || c==153

		){
			// contiamo con il contatore n il numero di lettere della parola che stiamo leggendo
			n++;
		}
	}while (c != EOF);
	di.max_word_length = max_word_length;
	di.total_words= total_words;
	fclose(file);
	return di;
}

int create_dictionary(const char * filename, int multiprocess){
	/*	Each phrase is terminated by . ? ! or EOF 
	 *	. ? and ! are words
	 *	every other character sequence between spaces and/or . ? ! is a word
	 *	the last word of the file has the first as "next"
	 *	all other characters are ignored except for a-zA-Z'
	 * */
	int mu = multiprocess;

	struct dictionary_info di;
	int c;
	di = scan_words(filename);

	pid_t pid_dictionary=-1;
	if(mu==1){
		// if user selected multiprocess we create the second process
		pid_dictionary = fork();
	}

	if( pid_dictionary==0 || mu==0){
		if(di.total_words>0 && di.max_word_length>0){
			/* printf(": = %d %c\n",128,128);
		exit(1); */
			FILE *file_catcher = fopen(filename, "r");
			int total_words = di.total_words; // computed number of words
			int max_word_length = di.max_word_length; // max length possible of a word
			// printf("tw %d - mwl %d\n",total_words,max_word_length);
			// array for the words in the text 
			// we add +1 to the max_word_length because we use '\0' as string's last character
			char dictionary[total_words][max_word_length+1];
			// int next_words[total_words][10][2];
			int ** next_words;
			next_words = malloc( sizeof(int*) * total_words);
			for(int i=0;i<total_words;i++){
				next_words[i] = malloc( sizeof(int) * 10 );
			}
			int ** frequence;
			frequence = malloc( sizeof(int*) * total_words);
			for(int i=0;i<total_words;i++){
				frequence[i] = calloc(1, sizeof(int) * 10);
			}
			// int next_words_unique_count[total_words];
			int * next_words_unique_count;
			next_words_unique_count = calloc(total_words, sizeof(int));
			int * next_words_total_count;
			next_words_total_count = calloc(total_words, sizeof(int));
			char * current_word = NULL;
			current_word = (char*) malloc(2*(max_word_length+1)*sizeof(char));
			// words is the counter for the words, n for the letters in a word
			int words=0, n=0, words_in_text=0;
			int last_word_index=0;
			do{
				// we scan each character of the file until the end of the file (EOF)
				c=fgetc(file_catcher);
				// if(c=='!')
				// printf("%d:%c n=%d - %s\n",c,c,n,current_word);
				// if n>0 means that we have already begun to write a word
				// so we terminate it with \0, we set n=0 for the next word and words++
				// and we skip to the next iteration of the while
				if( (n>0 && (c==' ' || c=='\n' || c=='\'')) || c==EOF || c=='?' || c=='!' || c=='.'){
					if(c=='\''){
						current_word[n++] = c;
					}
					// flag for telling if it's the end of the file and we finished
					int addword = 1;
					if( c==EOF && n==0 ){
						addword=0;
					}
					// if we encounter ? ! or . we must store it as a separate word, so we repeat the 
					// block responsible for storing the word
					int repeat = 1;
					if( (c=='?' || c=='!' || c=='.')){
						if(n>0){ 
							repeat = 2;
						}else{
							current_word[n++] = c;
						}
					}
					// if(c=='!') printf("n %d - repeat %d - cw %s\n",n,repeat,current_word);
					for(int r=0;r<repeat;r++){
						int current_index = words;
						if(addword==1){
							// we need to take in the current_word ! ? or .
							if(r>0){
								current_word[n++] = c;
							}
							// we close the word with the terminator \0
							current_word[n++] = '\0';
							words_in_text++;

							// we determine if the world is already in the dictionary
							int check=0;
							for(int w=0;w<words;w++){
								// printf("%s\n",current_word);
								check = compare_words(dictionary[w], current_word);
								if(check>0){
									// printf("Found %s --> %s\n",dictionary[w],current_word);
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
						}else{
							current_index = 0;
						}
						// we now store the current_word as successor of the previous
						// we use the index of the dictionary as reference
						if(words_in_text>1){ 
							next_words_total_count[ last_word_index ] ++;
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
									int * new_frequence = realloc(frequence[ last_word_index ], sizeof(int) * next_size);
									/* int next_size = ( sizeof( next_words_unique_count[ last_word_index ] ) + 1 ) * sizeof(int);
								int * new_next_word = realloc(next_words[ last_word_index ], next_size);
								int * new_frequence = realloc(frequence[ last_word_index ], next_size); */
									// printf("realloc next words\n");
									// if(new_next_word==NULL){
									if(new_next_word==NULL || new_frequence==NULL){
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
									frequence[ last_word_index ] = new_frequence;
								}
								next_words[ last_word_index ][ next_words_unique_count[ last_word_index ] ] = current_index;
								frequence[ last_word_index ][ next_words_unique_count[ last_word_index ] ] = 0;
								next_words_unique_count[ last_word_index ]++;
							}
							frequence[ last_word_index ][ current_nw_index ] ++;
						}
						last_word_index = current_index;
						// printf("%d - %s\n",current_index,dictionary[current_index]);
					}
				}else if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') // || c=='\''
					|| c==195 
				){
					// printf("%d:%c n=%d - %s\n",c,c,n,current_word);
					// if c is a "valid" character we put it in the dictionary
					// printf("%c - %d\n",c,c);

					if(c==195){
						int accented_char = c;

						c=fgetc(file_catcher);
						if( c==160  || c==168 || c==169 || c==172 || c==178 || c==185 ||
							c==128 || c==136 || c==137 || c==140 || c==146 || c==153){
							if(c>=128 && c<=159){
								c+=32;
							}
							current_word[n++]=accented_char;
							current_word[n++]=c;
						}
						continue;
					}
					// we consider all the upper case letters as lowercase
					if( (c>='A' && c<='Z') ){
						c+=32;
					}
					current_word[n] = (char) c;
					n++;
				}
			}while (c != EOF);
			// we must set here the first word as the next of the last
			fclose(file_catcher);

			pid_t pid_write = -1;
			if(mu==1){
				// if user selected multiprocess we create the third process
				pid_write = fork();
			}

			// if user has not selected multiprocess or this is the child
			if(pid_write==0 || mu==0){
				char* filename_out = "dictionary.csv";
				FILE *fpout;
				if ((fpout=fopen(filename_out,"wt"))==NULL) {
					printf("Wrong output file name");
					exit(1);
				}

				for(int i=0;i<words;i++){
					fprintf(fpout,"%s",dictionary[i]);
					float sum=0;
					for(int a=0;a<next_words_unique_count[i];a++){
						float perc;
						if(a< (next_words_unique_count[i]-1) ){
							perc=(float) frequence[i][a]/ (float) next_words_total_count[i];
							int conv = (int) ( perc * 10000);
							perc = (float) conv / 10000;
							sum+=perc;
							// printf("%.4g ===> %d => %.4g\n",perc,conv,sum);
						}else{
							// if it is the last next_word, we make sure that the sum of all the frequences is 1
							// this is necessary due to the cut on the 4th decimal digit
							perc = 1-sum;
						}
						// printf("%s ==> perc = %.4g sum = %.4g\n",dictionary[i],perc,sum);
						fprintf(fpout,",%s,%.4g",dictionary[ next_words[i][a] ],perc);
					}
					fprintf(fpout,"\n");
				}
				fclose(fpout);
			}

			// ##### We free the memory (in both parent and child process if mu selected)
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
			// we check if the second fork failed
			if(pid_write<0 && mu==1){
				printf("Impossible to fork write\n");
				exit(1);
			}
		}else{
			printf("Invalid file content\n");
			exit(1);
		}
	}
	// we check if the first fork failed
	if(pid_dictionary<0 && mu==1){
		printf("Impossible to fork dictionary\n");
		exit(1);
	}
	return 0;
}

