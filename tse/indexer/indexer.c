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

#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdio.h>
#include "pageio.h"
#include "webpage.h"  
#include "pageio.h"   
#include "hash.h"

typedef struct {
    char *w;   // normalized word (heap copy)
    int   count;  // frequency on page 1
} wordcount_t;

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
//for hsearch
 bool match_word(void *elem, const void *key) 
 {
    const wordcount_t *wc = (const wordcount_t*)elem;
    const char *k = (const char*)key;
    return strcmp(wc->w, k) == 0;
}


void add_or_increment(hashtable_t *ht, const char *norm_word) 
{
    wordcount_t *wc = hsearch(ht, match_word, norm_word, (int)strlen(norm_word));
    if (wc != NULL) 
	{
        wc->count++;
    }

	else 
	{
        wc = malloc(sizeof(*wc));
        wc->w = strdup(norm_word);
        wc->count = 1;
        hput(ht, wc, wc->w, (int)strlen(wc->w));
    }
}

//get the total count  
static int g_total = 0;
static void sum_counts(void *elem) 
{
    g_total += ((wordcount_t*)elem)->count;
}
	
int main(int argc, char **argv) {
	// Defaults
	const char *pagedir = "../crawler/pagedir";
	int id = 1;
	if (argc > 1) 
	{
		pagedir = argv[1];
	}
	if (argc > 2) 
	{
		id = atoi(argv[2]);
	}

	webpage_t *page = pageload(id, (char*)pagedir);
	if (page == NULL) {
		fprintf(stderr, "error!\n");
		return 1;
	}
	int pos = 0;
	char *word = NULL;

	int stream_count = 0;  
	hashtable_t *ht = hopen(1000);

	while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
		if (NormalizeWord(word) == 1) 
		{
			printf("%s\n", word);
		}
		free(word);
	}
	webpage_delete(page);
	
	g_total = 0;
    happly(ht, sum_counts);

	// Report & check
    printf("Sum of counts over hash table: %d\n", g_total);
    printf("Stream normalized word count : %d\n", stream_count);
    printf("Check (sum == stream): %s\n", (g_total == stream_count) ? "YES" : "NO");

	hclose(ht);

	return 0;
}

