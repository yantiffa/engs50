/* 
 * lhash.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 11-08-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "queue.h"
#include "lhash.h"
#include "hash.h"

lhashtable_t * lhopen(uint32_t hsize){
	lhashtable_t *lhash = malloc(sizeof(lhashtable_t));
	if (lhash == NULL) return NULL;
	lhash->ht = hopen(hsize);
	if (lhash ->ht == NULL){
		free(lhash);
		return NULL;
	}

	if (pthread_mutex_init(&lhash->m, NULL) != 0) {
		hclose(lhash->ht);
		free(lhash);
		return NULL;
	}
	return lhash;
}

void lhclose(lhashtable_t *lhtp){
	if (lhtp == NULL) return;
	pthread_mutex_lock(&lhtp->m);
	hclose(lhtp->ht);
	pthread_mutex_unlock(&lhtp->m);
	pthread_mutex_destroy(&lhtp->m);  
	free(lhtp);
}

int lhput(lhashtable_t *lhtp, void *elementp, const char *key, int keylen){
	if (lhtp == NULL) return -1;
	pthread_mutex_lock(&lhtp->m);
	int rc = hput(lhtp->ht, elementp, key, keylen);
	pthread_mutex_unlock(&lhtp->m);
	return rc;
}

void lhapply(lhashtable_t *lhtp, void (*fn)(void *ep)){
	if (lhtp == NULL || fn == NULL) return;
	pthread_mutex_lock(&lhtp->m);
	happly(lhtp->ht, fn);
	pthread_mutex_unlock(&lhtp->m);
}

void *lhsearch(lhashtable_t *lhtp,
              bool (*searchfn)(void *elementp, const void *searchkeyp),
              const char *key,
							 int32_t keylen){
	if (lhtp == NULL || searchfn == NULL) return NULL;
	pthread_mutex_lock(&lhtp->m);
	void *result = hsearch(lhtp->ht, searchfn, key, keylen);
	pthread_mutex_unlock(&lhtp->m);
	return result;
}

void *lhremove(lhashtable_t *lhtp,
              bool (*searchfn)(void *elementp, const void *searchkeyp),
              const char *key,
							 int32_t keylen) {
	if (lhtp == NULL || searchfn == NULL) return NULL;
	pthread_mutex_lock(&lhtp->m);
	void *result = hremove(lhtp->ht, searchfn, key, keylen);
	pthread_mutex_unlock(&lhtp->m);
	return result;
}
