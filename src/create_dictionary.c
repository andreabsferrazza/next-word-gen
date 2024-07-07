
int create_dictionary(){
	FILE *file = fopen("./test/input", "r");
	char *code;
	size_t n = 0;
	int c;
	char space = ' ';

	if (file == NULL)
		return 1; 

	int sizecode = 1000;
	// TODO gestire varie grandezze files
	code = malloc(sizecode);
	char words_array[sizecode][sizecode];
	int words = 0;

	/*	Each phrase is terminated by . ? ! or EOF 
	 *	. ? and ! are words
	 *	every other character sequence between spaces and/or . ? ! is a word
	 *	the last word of the file has the first as "next"
	 * */
	do{
		c=fgetc(file);
		if(n>0 && (c==space || c==EOF)){
			for(int i=0;i<n;i++){
				words_array[words][i] = code[i];
			}
			words++;
			n=0;
			continue;
		}
		if(c=='?' || c=='!' || c=='.'){
			for(int i=0;i<n;i++){
				words_array[words][i] = code[i];
			}
			words++;
			words_array[words][0] = c;
			words++;
			n=0;
			continue;
		}
		if( (c>='a' && c<='z') || (c>='A' && c<='Z') || c=='\''){
			code[n++] = (char) c;
		}
	}while (c != EOF);

	code[n] = '\0';
	for(int i=0;i<words;i++){
		printf("%s\n",words_array[i]);
	}
	return 0;
}

