/* 
 * indexer.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 10-21-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <ctype.h>
#include <stdio.h>
#include "pageio.h"

int NormalizeWord(char *word) {
	// if word is empty
	if (word == NULL){
		return 0;
	}

	// check for length
	int lens = strlen(word);
	if (lens < 3){
		return 0;
	}
	for (int i = 0; i < lens; i++) {
		if (!isalpha(word[i])) {
			return 0;
		}
		word[i] = tolower(word[i]);
	}
	return 1;
}



	
	
int main() {
	webpage_t *page = pageload(1, "../pages");
	if (page == NULL) {
		fprintf(stderr, "error!\n");
		return 1;
	}
	int pos = 0;
	char *word;

	while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
		if (NormalizeWord(word) == 1) {
			printf("%s\n", word);
		}
		free(word);
	}
	webpage_delete(page);
	
	return 0;
}

