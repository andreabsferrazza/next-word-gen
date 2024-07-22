#include "functions.h"
/*
 * This function compares two strings a and b (input)
 * a and b must be terminated with the character \0
 * OUTPUT
 * 0 if a and b are different
 * >0 if a and b are equal
 * */
int compare_words(char* a, char* b){
	int z=0, check=0;
	// if at least one character is different the word is different
	while(a[z]!='\0' && b[z]!='\0'){
		if(a[z]!=b[z]){
			check=0;
			break;
		}
		check++;
		z++;
	}
	if(a[z]!=b[z]) check=0;
	return check;
}
