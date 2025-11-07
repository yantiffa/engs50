// lqtest_dual.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lqueue.h"

#define N_PER_THREAD 10000

typedef struct {
    lqueue_t *lq;
    int       tid;   
} thread_arg_t;

typedef struct {
    int value;
} item_t;

void *producer(void *arg) {
    thread_arg_t *targ = arg;
    for (int i = 0; i < N_PER_THREAD; i++) {
        item_t *it = malloc(sizeof(item_t));
        it->value = targ->tid * N_PER_THREAD + i;

        if (lqput(targ->lq, it) != 0) {
            fprintf(stderr, "lqput failed in thread %d\n", targ->tid);
        }
    }
    return NULL;
}

int main(void) {
    lqueue_t *lq = lqopen();
    if (lq == NULL) {
        fprintf(stderr, "lqopen failed\n");
        return 1;
    }

    pthread_t t1, t2;
    thread_arg_t a1 = { .lq = lq, .tid = 1 };
    thread_arg_t a2 = { .lq = lq, .tid = 2 };

    // start two producer threads
    pthread_create(&t1, NULL, producer, &a1);
    pthread_create(&t2, NULL, producer, &a2);

    // wait for them to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // now drain the queue
    int count = 0;
    item_t *it;
    while ((it = lqget(lq)) != NULL) {
        count++;
        free(it);
    }

    printf("Got %d items, expected %d\n", count, 2 * N_PER_THREAD);

    lqclose(lq);
    return 0;
}
