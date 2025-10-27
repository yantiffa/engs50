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
#include <dirent.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pageio.h"
#include "webpage.h"  
#include "pageio.h"   
#include "hash.h"
#include "queue.h"
#include "indexio.h"

typedef struct {
    int doc_id;
    int count;
} doc_count_t;

// Structure for index entries - word mapped to queue of documents
typedef struct {
    char *word;           // normalized word 
    queue_t *doc_queue;   // queue of doc_count_t entries
} index_entry_t;

static int g_total = 0;

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
    const index_entry_t *entry = (const index_entry_t*)elem;
    const char *k = (const char*)key;
    return strcmp(entry->word, k) == 0;
}

bool match_doc(void *elem, const void *key) 
{
    const doc_count_t *dc = (const doc_count_t*)elem;
    const int *doc_id = (const int*)key;
    return dc->doc_id == *doc_id;
}

static void free_doc(void *elem) {
	free(elem);
}

static void free_each(void *elem) {
	index_entry_t *entry = (index_entry_t*)elem;
	qapply(entry->doc_queue, free_doc);
	qclose(entry->doc_queue);
	free(entry->word);
	free(entry);	
}
	




void add_or_increment(hashtable_t *ht, const char *norm_word, int doc_id) {
    index_entry_t *entry = hsearch(ht, match_word, norm_word, (int)strlen(norm_word));
    
    if (entry != NULL) {
        // Word exists, check if document exists in queue
        doc_count_t *dc = qsearch(entry->doc_queue, match_doc, &doc_id);
        if (dc != NULL) 
		{
            dc->count++;
        } 
		else 
		{
            dc = malloc(sizeof(*dc));
            dc->doc_id = doc_id;
            dc->count = 1;
            qput(entry->doc_queue, dc);
        }
    } else {
        // Word doesn't exist, create new entry
        entry = malloc(sizeof(*entry));
        entry->word = strdup(norm_word);
        entry->doc_queue = qopen();
        
        doc_count_t *dc = malloc(sizeof(*dc));
        dc->doc_id = doc_id;
        dc->count = 1;
        qput(entry->doc_queue, dc);
        
        hput(ht, entry, entry->word, (int)strlen(entry->word));
    }
}

int main(int argc, char **argv) {
	// condition check
	if (argc != 3) {
		fprintf(stderr, "wrong number of commands given:(\n");
		return 1;
	}
	const char *pagedir = argv[1];
	const char *indexnum = argv[2];

	DIR *dir = opendir(pagedir);
	if (!dir) {
		fprintf(stderr, "no directory\n");
		return 1;
	}else{
		closedir(dir);
	}
	

	hashtable_t *ht = hopen(1000);
	int stream_count = 0;
	int id = 1;

	while (true) {
		webpage_t *page = pageload(id, (char*)pagedir);
		if (page == NULL) {
			break;
		}
		int pos = 0;
		char *word = NULL; 
		
		while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
			if (NormalizeWord(word) == 1) 
				{
					add_or_increment(ht, word, id);
					stream_count++;
				}
			free(word);
		}
		webpage_delete(page);
		id++;
	}	
	indexsave(indexnum, ht);
	happly(ht, free_each);

	hclose(ht);

	return 0;
}

