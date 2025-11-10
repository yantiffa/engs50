#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"
#include "pageio.h"

// Shared resources
typedef struct {
    queue_t *queue;
    hashtable_t *visited;
    char *pagedir;
    int maxdepth;
    pthread_mutex_t queue_mutex;
    pthread_mutex_t hash_mutex;
    pthread_mutex_t id_mutex;
    int page_id;
    int active_threads;
    pthread_mutex_t active_mutex;
    pthread_cond_t active_cond;
} shared_data_t;

void print_webpage(void* elementp) {
    webpage_t *page = (webpage_t*)elementp;
    printf("URL: %s, Depth: %d\n", webpage_getURL(page), webpage_getDepth(page));
}

static void usage(const char* program_name) 
{
    fprintf(stderr, "usage: %s <seedurl> <pagedir> <maxdepth> <numthreads>\n", program_name);
}

static bool same_url(void *elementp, const void *searchkeyp)
{
    const char *stored_url = (const char*)elementp;
    const char *want_url   = (const char*)searchkeyp;
    return stored_url && want_url && strcmp(stored_url, want_url) == 0;
}

void* crawler_thread(void *arg) {
    shared_data_t *shared = (shared_data_t*)arg;
    
    while (true) {
        webpage_t *curr = NULL;
        
        // Try to get a page from the queue
        pthread_mutex_lock(&shared->queue_mutex);
        curr = qget(shared->queue);
        pthread_mutex_unlock(&shared->queue_mutex);
        
        if (curr == NULL) {
            // Check if any threads are still working
            pthread_mutex_lock(&shared->active_mutex);
            if (shared->active_threads == 0) {
                // No active threads and queue is empty - we're done
                pthread_mutex_unlock(&shared->active_mutex);
                break;
            }
            // Other threads still working, wait a bit and try again
            pthread_mutex_unlock(&shared->active_mutex);
            usleep(10000); // 10ms
            continue;
        }
        
        // Mark this thread as active
        pthread_mutex_lock(&shared->active_mutex);
        shared->active_threads++;
        pthread_mutex_unlock(&shared->active_mutex);
        
        // Fetch the page
        if (!webpage_fetch(curr)) {
            webpage_delete(curr);
            // Mark thread as inactive
            pthread_mutex_lock(&shared->active_mutex);
            shared->active_threads--;
            pthread_cond_broadcast(&shared->active_cond);
            pthread_mutex_unlock(&shared->active_mutex);
            continue;
        }
        
        // Save the page with a unique ID
        pthread_mutex_lock(&shared->id_mutex);
        int id = shared->page_id++;
        pthread_mutex_unlock(&shared->id_mutex);
        pagesave(curr, id, shared->pagedir);
        
        // Extract URLs if depth allows
        int depth = webpage_getDepth(curr);
        if (depth < shared->maxdepth) {
            int pos = 0;
            char *result = NULL;
            while ((pos = webpage_getNextURL(curr, pos, &result)) > 0) {
                if (result && IsInternalURL(result)) {
                    // Check if URL has been visited
                    pthread_mutex_lock(&shared->hash_mutex);
                    bool found = (hsearch(shared->visited, same_url, result, (int)strlen(result)) != NULL);
                    
                    if (!found) {
                        char *copy = strdup(result);
                        if (copy != NULL) {
                            hput(shared->visited, copy, copy, (int)strlen(copy));
                            pthread_mutex_unlock(&shared->hash_mutex);
                            
                            webpage_t *child_page = webpage_new(result, depth + 1, NULL);
                            if (child_page) {
                                pthread_mutex_lock(&shared->queue_mutex);
                                qput(shared->queue, child_page);
                                pthread_mutex_unlock(&shared->queue_mutex);
                            }
                        } else {
                            pthread_mutex_unlock(&shared->hash_mutex);
                        }
                    } else {
                        pthread_mutex_unlock(&shared->hash_mutex);
                    }
                }
                free(result);
                result = NULL;
            }
        }
        webpage_delete(curr);
        
        // Mark thread as inactive
        pthread_mutex_lock(&shared->active_mutex);
        shared->active_threads--;
        pthread_cond_broadcast(&shared->active_cond);
        pthread_mutex_unlock(&shared->active_mutex);
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 5) { 
        usage(argv[0]); 
        return -1;
    }

    char *seed     = argv[1];
    char *pagedir  = argv[2];
    char *depthstr = argv[3];
    char *threadstr = argv[4];

    char *endp = NULL;
    long maxdepth = strtol(depthstr, &endp, 10);
    long numthreads = strtol(threadstr, &endp, 10);
    
    if (numthreads <= 0) {
        fprintf(stderr, "Error: number of threads must be positive\n");
        return -1;
    }

    // Initialize shared data
    shared_data_t shared;
    shared.queue = qopen();
    shared.visited = hopen(107);
    shared.pagedir = pagedir;
    shared.maxdepth = (int)maxdepth;
    shared.page_id = 1;
    shared.active_threads = 0;
    
    pthread_mutex_init(&shared.queue_mutex, NULL);
    pthread_mutex_init(&shared.hash_mutex, NULL);
    pthread_mutex_init(&shared.id_mutex, NULL);
    pthread_mutex_init(&shared.active_mutex, NULL);
    pthread_cond_init(&shared.active_cond, NULL);

    // Add seed URL
    char *seed_copy = strdup(seed);
    hput(shared.visited, seed_copy, seed_copy, (int)strlen(seed_copy));
    webpage_t *page = webpage_new(seed, 0, NULL);
    qput(shared.queue, page);

    // Create threads
    pthread_t *threads = malloc(numthreads * sizeof(pthread_t));
    for (int i = 0; i < numthreads; i++) {
        pthread_create(&threads[i], NULL, crawler_thread, &shared);
    }

    // Wait for all threads to complete
    for (int i = 0; i < numthreads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Saved %d page(s) to %s\n", shared.page_id - 1, pagedir);

    // Cleanup
    free(threads);
    qclose(shared.queue);
    hclose(shared.visited);
    pthread_mutex_destroy(&shared.queue_mutex);
    pthread_mutex_destroy(&shared.hash_mutex);
    pthread_mutex_destroy(&shared.id_mutex);
    pthread_mutex_destroy(&shared.active_mutex);
    pthread_cond_destroy(&shared.active_cond);

    exit(EXIT_SUCCESS);
}
