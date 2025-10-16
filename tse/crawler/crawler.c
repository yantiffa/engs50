#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../webpage.h"
#include "queue.h"

void print_webpage(void* elementp) {
    webpage_t *page = (webpage_t*)elementp;
    printf("URL: %s, Depth: %d\n", webpage_getURL(page), webpage_getDepth(page));
}

int main() {
	webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	if(!webpage_fetch(page)) {
		printf("Failed to fetch webpage\n");
		webpage_delete(page);
		exit(EXIT_FAILURE);
	}
	//step3: with queue
	queue_t * queue = qopen();
	int pos = 0;
	char *result;
	while ((pos = webpage_getNextURL(page, pos, &result)) > 0) {
		if(IsInternalURL(result)){
			webpage_t *child_page = webpage_new(result, 1, NULL);
			qput(queue, child_page);
		}
		free(result);
	}
	webpage_delete(page);
	qapply(queue, print_webpage);
	qapply(queue, webpage_delete);
	qclose(queue);
	exit(EXIT_SUCCESS);
} 
