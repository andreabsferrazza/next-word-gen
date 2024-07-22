#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
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
		}else if(c>0){
			// if the current character is a valid one
			n++;
		}
	}while (c != EOF);
	fclose(file);
	pti.total_words = total_words;
	pti.max_word_length = max_word_length;
	return pti;
}

int generate_random_text(const char* filename, int words_number, const char * first_word, int multiprocess){
	int mu = multiprocess;
	// ###################### 1. we scan the file to determine the max_word_length and the total_words in the dictionary
	struct ptable_info pti; 
	pti = scan_dictionary(filename);
	int max_word_length = pti.max_word_length;
	int total_words = pti.total_words;

	if(max_word_length==0 || total_words==0){
		printf("Invalid dictionary\n");
		exit(1);
	}
	if(words_number<=0 || words_number>(1073741824/max_word_length)){
		printf("Words number too big\n");
		exit(1);
	}

	pid_t pid_parse = -1;
	if(mu==1){
		pid_parse = fork();
	}

	if(mu==0 || pid_parse==0){
		// ###################### 2. we parse the csv
		// declarations+initialize
		int c;
		int words=0,n=0;
		int word_identifier=0;
		char dictionary[total_words][max_word_length+1];
		char next_words[total_words][total_words+1][max_word_length+1];
		int next_words_indexes[total_words][total_words];
		char next_words_probability[total_words][total_words][6]; // max digits 0.0001
		int * next_words_counter;
		next_words_counter = calloc(total_words, sizeof(int));
		// initializing next_word_counter elements to zero
		char punctuation_indexes[3]; // ? ! .
		int pi=0; // punctuation count
		FILE *file;
		if ((file=fopen(filename,"r"))==NULL) {
			printf("Wrong input file name\n");
			exit(1);
		}
		do{
			// we scan the file 
			c=fgetc(file);
			// if we already encountered a character (n>0) that is not the EOF, '\n' or ',' 
			if(n>0 && (c==',' || c=='\n' || c==EOF)){
				// we have found a word | word_identifier==0 when we have found the "main" word of the line
				if(word_identifier==0){
					dictionary[words][n]='\0';
				}
				if(word_identifier>0){
					// in the odd positions between commas there are the
					// next words of the "main" word
					if(word_identifier%2==1){
						next_words[words][ next_words_counter[words] ][n]='\0';
					}else{
						// ... and in the even positions the probability
						next_words_probability[words][ next_words_counter[words] ][n]='\0';
						next_words_counter[words]++;
					}
				}
				// adjusting the counters
				if(c=='\n'){
					word_identifier=0;
					words++;
				}else{
					word_identifier++;
				}
				n=0;
			}else if(c>0){
				// we assume we have always "valid" characters in the csv
				if(word_identifier==0){
					dictionary[words][n] = c;
					if(c=='?' || c=='!' || c=='.'){
						// we know that ?!. can only appear once in the dictionary
						// so we take advantage of this and we "save" their indexes for later
						punctuation_indexes[pi] = words;
						pi++;
					}
				}
				if(word_identifier>0){
					// in the odd positions between commas there are the
					// next words of the "main" word
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

		// ###################### 3. now we determine the index for every next_word
		for(int i=0;i<total_words;i++){
			for(int j=0;j<next_words_counter[i];j++){
				for(int w=0;w<total_words;w++){
					int check = compare_words(dictionary[w], next_words[i][j]);
					if(check>0){
						next_words_indexes[i][j] = w;
						break;
					}
				}
			}
		}


		// ###################### 4. now we determine the first word

		char* filename_out = "output.txt";
		FILE *fpout;
		if ((fpout=fopen(filename_out,"w"))==NULL) {
			printf("Wrong output file name");
			exit(1);
		}

		int first_word_index = 0;
		char * fw = (char*) first_word;
		srand(time(NULL));   // Init
		int found=0;
		// if fw!=" " the user have submitted an input word, so we search it in the dictionary
		if( compare_words(fw," ")==0){
			for(int i=0;i<total_words;i++){
				int check = compare_words(dictionary[i],fw);
				if(check>0){
					found=1;
					first_word_index=i;
					break;
				}
			}
		}
		// int consider_first_print = 0;
		if(found==0){
			// we have not found the word if submitted (or not)
			// so we randomize a punctuation
			int punctuation = 0 + rand() / (RAND_MAX / (2 + 1) + 1);
			first_word_index = punctuation_indexes[punctuation];
		}

		pid_t pid_write = -1;
		if(mu==1){
			pid_write = fork();
		}

		if(mu==0 || pid_write==0){
			// if no word was found, we would start as we already written a punctuation (but we didn't)
			// if the word was found we set it as the last_index (last word)
			int last_index = first_word_index;
			// we cycle until we printed all words_number words
			for(int w=0;w<words_number;w++){
				// we set the needle for the randomization
				int random_needle = (rand() % (10000 - 0 + 1)) + 0;
				int min_step = 0;

				// we determine if the last word was punctuation
				int last_was_punctuation=0;
				for(int p=0;p<pi;p++){
					if(punctuation_indexes[p]==last_index){
						last_was_punctuation=1;
					}
				}

				// we write the space after each word after the first
				if(w>0){
					fprintf(fpout," ");
				}

				// we determine the next_word
				for(int j=0;j<next_words_counter[last_index];j++){
					// we incrementally sum the previous probabilities and we check if the needle is in the current range
					// if it is we set that word as the next
					float current_step = ( atof( next_words_probability[last_index][j] ) * 10000) + min_step;
					if( (j==next_words_counter[last_index]-1) || // if we reached the last "next_word" we pick it
						(random_needle>=min_step && random_needle<current_step) // we check if the needle is in the current range
					){
						int new_index = next_words_indexes[last_index][j];

						// we check if we need to put the first letter uppercase and then we write the word on the file
						if(last_was_punctuation==1 || w==0){
							int z=0;
							do{
								int c = dictionary[ new_index ][z];
								if(z==0 && (c>='a' && c<='z') ){
									fprintf(fpout,"%c", dictionary[ new_index ][z]-32);
								}else{
									fprintf(fpout,"%c", dictionary[ new_index ][z]);
								}
								z++;
							}while(dictionary[ new_index ][z]!='\0');
						}else{
							fprintf(fpout,"%s",dictionary[ new_index ]);
						}
						// we update the index and go the next "main" word
						last_index = new_index;
						break;
					}
					// if the needle was not in the current range we go to the next one
					min_step = current_step;
				}
			}
			fprintf(fpout,"\n");
			fclose(fpout);
		}
		if(pid_write<0 && mu==1){
			printf("Impossible to fork write process\n");
			exit(1);
		}
	}
	if(pid_parse<0 && mu==1){
		printf("Impossible to fork parse process\n");
		exit(1);
	}
	return 0;
}
