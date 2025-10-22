#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"
#include "pageio.h"

void print_webpage(void* elementp) {
    webpage_t *page = (webpage_t*)elementp;
    printf("URL: %s, Depth: %d\n", webpage_getURL(page), webpage_getDepth(page));
}


static void usage(const char* program_name) 
{
    fprintf(stderr, "usage: %s <seedurl> <pagedir> <maxdepth>\n", program_name);
}
	
// Used in hsearch: compare a stored webpage_t*'s URL to a URL string
static bool same_url(void *elementp, const void *searchkeyp)
{
    const char *stored_url = (const char*)elementp;
    const char *want_url   = (const char*)searchkeyp;
	//make sure none of stored_url and want url is 0, then compare
    return stored_url && want_url && strcmp(stored_url, want_url) == 0;
}

int main(int argc, char *argv[]) {

	if (argc != 4) 
	{ 
		usage(argv[0]); 
		return -1;
	}

    char *seed     = argv[1];
    char *pagedir  = argv[2];
    char *depthstr = argv[3];

	char *endp = NULL;
	long maxdepth = strtol(depthstr, &endp, 10);

	//step5: store a copy of the file
	hashtable_t*visited = hopen(107);
	char *seed_copy = strdup(seed);
	hput(visited, seed_copy, seed_copy, (int)strlen(seed_copy));
	
	queue_t * queue = qopen();
	webpage_t *page = webpage_new(seed, 0, NULL);
	qput(queue, page);
	//step3: with queue
	
	webpage_t *curr;
	int id = 1;

	while ((curr = qget(queue)) != NULL) 
	{
		//skip failures instead of exiting
		if (!webpage_fetch(curr)) 
		{
			webpage_delete(curr);
			continue;
		}
		pagesave(curr, id++, pagedir);
	
		int depth = webpage_getDepth(curr);
		if (depth < maxdepth) 
		{
			int pos = 0;
			char *result = NULL;
			while ((pos = webpage_getNextURL(curr, pos, &result)) > 0) 
			{
				if (result && IsInternalURL(result)) 
				{
					
					if (hsearch(visited, same_url, result, (int)strlen(result)) == NULL)
					{
					
						char *copy = strdup(result);
						if (copy != NULL) 
						{
							hput(visited, copy, copy, (int)strlen(copy));
							webpage_t *child_page = webpage_new(result, depth + 1, NULL);
							if (child_page) qput(queue, child_page);
						}
					}
				}
				free(result);
				result = NULL;
			}
		}
		webpage_delete(curr);
	}

	// close the queue now that all elements have been removed and freed
	qclose(queue);
	// close the hashtable
	hclose(visited);
	exit(EXIT_SUCCESS);
	printf("Saved %d page(s) to %s\n", id-1, pagedir);
	
	
} 
 
