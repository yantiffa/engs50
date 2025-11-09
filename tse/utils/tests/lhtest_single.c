/* 
 * lhtest_single.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 11-08-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "lhash.h"
#include <string.h>

typedef struct {
	int value;
	char *key;
} item_t;

void print_item(void *ep) {
	item_t *ip = ep;
	printf("[%s: %d] ", ip->key, ip->value);
}

bool search_item(void *elementp, const void *keyp){
	item_t *item = (item_t *)elementp;
	const char *search_key = (const char *)keyp;
	return strcmp(item->key, search_key) == 0;
}
	

int main(void) {
	lhashtable_t *lhtp = lhopen(100);
	if (lhtp == NULL) {
		fprintf(stderr, "lqopen failed\n");
		return 1;
	}
	
	// put 3 items
	for (int i = 1; i <= 3; i++) {
		item_t *it = malloc(sizeof(item_t));
		it->key = malloc(10);
		sprintf(it->key, "key%d", i);
		it->value = i*1000;
		if (lhput(lhtp, it, it->key, strlen(it->key)) != 0) {
			fprintf(stderr, "lput failed");
		}
	}

	lhapply(lhtp, print_item);

	for (int i = 1; i <= 3; i++) {
		char key[10];
		sprintf(key, "key%d", i);
		item_t *found = lhsearch(lhtp, search_item, key, strlen(key));
		if (!found) {
		  fprintf(stderr, "lsearch failed");
		}
	}

	for (int i = 1; i <= 3; i++) {
		char key[10];
		sprintf(key, "key%d", i);
		item_t *removed = lhremove(lhtp, search_item, key, strlen(key));
		if (!removed){
			fprintf(stderr, "lremove failed");
		}
		else{
			free(removed ->key);
			free(removed);
		}
	}
	lhclose(lhtp);
	printf("Test completed successfully\n");
	return 0;
}
