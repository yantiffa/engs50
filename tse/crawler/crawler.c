#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"

void print_webpage(void* elementp) {
    webpage_t *page = (webpage_t*)elementp;
    printf("URL: %s, Depth: %d\n", webpage_getURL(page), webpage_getDepth(page));
}

int32_t pagesave(webpage_t *pagep, int id, char *dirname) {
	char path[256];
	sprintf(path, "%s/%d", dirname, id);
	FILE *fp = fopen(path, "w");

	if (fp==NULL) {
		//error
		return -1;
	}
	fprintf(fp, "%s\n", webpage_getURL(pagep));
	fprintf(fp, "%d\n", webpage_getDepth(pagep));
	fprintf(fp, "%d\n", webpage_getHTMLlen(pagep));
	fprintf(fp, "%s\n", webpage_getHTML(pagep));
	
	fclose(fp);
	return 0;
}
	
// Used in hsearch: compare a stored webpage_t*'s URL to a URL string
static bool same_url(void *elementp, const void *searchkeyp) 
{
    const char *stored_url = webpage_getURL((webpage_t*)elementp);
    const char *want_url   = (char*)searchkeyp;
	//make sure none of stored_url and want url is 0, then compare
    return stored_url && want_url && strcmp(stored_url, want_url) == 0; 
}

int main() {
	char*seed = "https://thayer.github.io/engs50/";
	webpage_t *page = webpage_new(seed, 0, NULL);
	if(!webpage_fetch(page)) {
		printf("Failed to fetch webpage\n");
		webpage_delete(page);
		exit(EXIT_FAILURE);
	}

	//step5: store a copy of the file
	pagesave(page, 1, "../pages");
	hashtable_t*visited = hopen(107);
	hput(visited, page, seed, strlen(seed));


	//step3: with queue
	queue_t * queue = qopen();
	int pos = 0;
	char *result;
	int depth = webpage_getDepth(page) + 1;
	while ((pos = webpage_getNextURL(page, pos, &result)) > 0) 
	{
        if (IsInternalURL(result)) 
		{
            if (hsearch(visited, same_url, result, (int)strlen(result)) == NULL) 
			{
                webpage_t *child_page = webpage_new(result, depth, NULL);
                if (child_page!=NULL) 
				{
                    qput(queue, child_page);                                // enqueue once
                    hput(visited, child_page, result, (int)strlen(result)); // remember seen
                }
            }
        }
        free(result);  // ALWAYS free what webpage_getNextURL allocates
        result = NULL;
    }

	webpage_delete(page);
	qapply(queue, print_webpage);
	qapply(queue, webpage_delete);
	qclose(queue);
	exit(EXIT_SUCCESS);
} 
