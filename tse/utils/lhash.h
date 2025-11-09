/* 
 * lhash.h --- 
 * 
 * Author: Tingwen Yan
 * Created: 11-08-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */

#ifndef _LHASH_H_
#define _LHASH_H_

#include <pthread.h>
#include <stdbool.h>
#include "hash.h"

typedef struct lhash {
	hashtable_t *ht;
	pthread_mutex_t m;
}lhashtable_t;

lhashtable_t * lhopen(uint32_t hsize);

void lhclose(lhashtable_t *lhtp);

int lhput(lhashtable_t *lhtp, void *elementp, const char *key, int keylen);

void lhapply(lhashtable_t *lhtp, void (*fn)(void *ep));

void *lhsearch(lhashtable_t *lhtp,
              bool (*searchfn)(void *elementp, const void *searchkeyp),
              const char *key,
              int32_t keylen);

void *lhremove(lhashtable_t *lhtp,
              bool (*searchfn)(void *elementp, const void *searchkeyp),
              const char *key,
              int32_t keylen);
