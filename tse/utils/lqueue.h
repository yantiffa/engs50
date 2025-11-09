#ifndef _LQUEUE_H_
#define _LQUEUE_H_

#include <pthread.h>
#include <stdbool.h>
#include "queue.h"

typedef struct lqueue {
    queue_t        *q;
    pthread_mutex_t m;
} lqueue_t;

/* Open a locked queue */
lqueue_t *lqopen(void);

/* Close a locked queue; frees everything */
void lqclose(lqueue_t *lqp);

/* Thread-safe put */
int lqput(lqueue_t *lqp, void *elementp);

/* Thread-safe get (returns NULL if empty) */
void *lqget(lqueue_t *lqp);

/* Thread-safe apply */
void lqapply(lqueue_t *lqp, void (*fn)(void *elementp));

/* Thread-safe search */
void *lqsearch(lqueue_t *lqp,
               bool (*searchfn)(void *elementp, const void *keyp),
               const void *skeyp);

#endif // _LQUEUE_H_
