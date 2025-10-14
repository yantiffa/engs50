/* 
 * queue.h --- 
 * 
 * Author: Tingwen Yan
 * Created: 10-04-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>
typedef void queue_t;
queue_t* qopen(void);
void qclose(queue_t *qp);
int32_t qput(queue_t *qp, void *elementp);
void* qget(queue_t *qp);
void qapply(queue_t *qp, void (*fn)(void* elementp));
void* qsearch(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp);
void* qremove(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp);
void qconcat(queue_t *q1p, queue_t *q2p);
