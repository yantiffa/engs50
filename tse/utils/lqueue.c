#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "lqueue.h"
#include "queue.h"

lqueue_t *lqopen(void) {
    lqueue_t *lqp = malloc(sizeof(lqueue_t));
    if (lqp == NULL) return NULL;

    lqp->q = qopen();
    if (lqp->q == NULL) {
        free(lqp);
        return NULL;
    }

    if (pthread_mutex_init(&lqp->m, NULL) != 0) {
        qclose(lqp->q);
        free(lqp);
        return NULL;
    }

    return lqp;
}

void lqclose(lqueue_t *lqp) {
    if (lqp == NULL) return;

    pthread_mutex_lock(&lqp->m);
    qclose(lqp->q);
    pthread_mutex_unlock(&lqp->m);

    pthread_mutex_destroy(&lqp->m);
    free(lqp);
}

int lqput(lqueue_t *lqp, void *elementp) {
    if (lqp == NULL) return -1;

    pthread_mutex_lock(&lqp->m);
    int rc = qput(lqp->q, elementp);
    pthread_mutex_unlock(&lqp->m);

    return rc;
}

void *lqget(lqueue_t *lqp) {
    if (lqp == NULL) return NULL;

    pthread_mutex_lock(&lqp->m);
    void *result = qget(lqp->q);
    pthread_mutex_unlock(&lqp->m);

    return result;
}

void lqapply(lqueue_t *lqp, void (*fn)(void *elementp)) {
    if (lqp == NULL || fn == NULL) return;

    pthread_mutex_lock(&lqp->m);
    qapply(lqp->q, fn);
    pthread_mutex_unlock(&lqp->m);
}

void *lqsearch(lqueue_t *lqp,
               bool (*searchfn)(void *elementp, const void *keyp),
               const void *skeyp) {
    if (lqp == NULL || searchfn == NULL) return NULL;

    pthread_mutex_lock(&lqp->m);
    void *result = qsearch(lqp->q, searchfn, skeyp);
    pthread_mutex_unlock(&lqp->m);

    return result;
}
