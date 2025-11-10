/* 
 * lhtest_dual.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 11-10-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lhash.h"
#include <string.h>

#define N_PER_THREAD 10000

typedef struct {
    lhashtable_t *lht;
    int       tid;   
} thread_arg_t;

typedef struct {
    char *key;
    int value;
} item_t;

bool search_item(void *elementp, const void *keyp){
	item_t *item = (item_t *)elementp;
	const char *search_key = (const char *)keyp;
	return strcmp(item->key, search_key) == 0;
}

void *producer(void *arg) {
    thread_arg_t *targ = arg;
    for (int i = 0; i < N_PER_THREAD; i++) {
        item_t *it = malloc(sizeof(item_t));
        it->key = malloc(20);
        sprintf(it->key, "t%d_i%d", targ->tid, i);
        it->value = targ->tid * N_PER_THREAD + i;

        if (lhput(targ->lht, it, it->key, strlen(it->key)) != 0) {
            fprintf(stderr, "lhput failed in thread %d\n", targ->tid);
        }
    }
    return NULL;
}

int main(void) {
    lhashtable_t *lht = lhopen(100);
    if (lq == NULL) {
        fprintf(stderr, "lqopen failed\n");
        return 1;
    }

    pthread_t t1, t2;
    thread_arg_t a1 = { .lht = lht, .tid = 1 };
    thread_arg_t a2 = { .lht = lht, .tid = 2 };

    // start two producer threads
    pthread_create(&t1, NULL, producer, &a1);
    pthread_create(&t2, NULL, producer, &a2);

    // wait for them to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    
    int count = 0;
    for (int tid = 1; tid < 3; tid++) {
      for (int i = 0; i < N_PER_THREAD; i++) {
        char key[20];
        sprintf(key, "t%d_i%d", tid, i);
        item_t *found = lhsearch(lht, search_item, key, strlen(key));
        if (found) {
          count++;
        }
      }
    }
    if (count == 2*N_PER_THREAD){
      printf("success!\n");
    }else{
      printf("fail\n");
    }
  
    lhclose(lht);
    return 0;
}

