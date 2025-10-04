/* 
 * queue.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 10-04-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct node {
	void *data;
	struct node *next;
}node;

typedef struct queue {
	node *start;
	node *end;
}queue_t;

/* create an empty queue */
queue_t* qopen(void){
	queue_t *qp = (queue_t*)malloc(sizeof(queue_t));
	if (qp==NULL){
		return NULL;
	}
	qp->start = NULL;
	qp->end = NULL;
	return qp;
}

/* deallocate a queue, frees everything in it */
void qclose(queue_t *qp) {
	if (qp  == NULL){
		return;
	}
	node *current = qp->start;
	while (current != NULL){
		node *temp = current->next;
		free(current);
		current = temp;
	}
	free(qp);
}

/*
	put element at the end of the queue
	returns 0 is successful; nonzero otherwise
*/
int32_t qput(queue_t *qp, void *elementp){
	if (qp == NULL){
		return -1;
	}

	node *store = (node*)malloc(sizeof(node));
	if (store == NULL){
		return -1;
	}
	store->data = elementp;
	store->next = NULL;

	if (qp->start == NULL) {
		qp->start = store;
		qp->end = store;
	}else {
		qp->end->next = store;
		qp->end = store;
	}
	return 0;
}

/* get the first element from queue, removing it from the queue */
void* qget(queue_t *qp) {
	if (qp == NULL){
		fprintf(stderr,"invalid input.\n");
		exit(EXIT_FAILURE);
	}
	if (qp->start == NULL) {
		fprintf(stderr,"invalid input.Queue is empty.\n");
		exit(EXIT_FAILURE);
	}
	void *info = qp->start->data;
	//special case when there is only one element in the queue
	if (qp->start == qp->end) {
		node *temp = qp->start;   
		qp->start = NULL;
		qp->end = NULL;
		free(temp);
		return info;
	}

	//all other cases
	node *temp1 = qp->start;
	qp->start = qp->start->next;
	free(temp1);
	return info;
}

/* apply a function to every element of the queue */
void qapply(queue_t *qp, void (*fn)(void* elementp)) {
	if (qp == NULL) {
		return;
	}
	node *current = qp->start;
	while (current != NULL) {
		fn(current->data);
		current = current ->next;
	}
}

/*search a queue using a supplied boolean function
	skeyp -- a key to search for
	searchfn -- a function applied to every element of the queue
	         -- elementp -- a pointer to an element
	         -- keyp - the key being searched for (i.e. will be set to skey at each step of the search)
					 -- returns TRUE or FALSE as defined in bool.h
	returns a pointer to an element, or NULL if not found
*/
void* qsearch(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {
	if (qp == NULL) {
		return NULL;
	}
	node *current = qp->start;
	while (current != NULL) {
		if (searchfn(current->data, skeyp)) {
				void *res = (void*)current->data;
				return res;
			}
		current = current->next;
	}
	return NULL;
}

/* search a queue using a supplied boolean function (as in qsearch),
	 removes the element from the queue and returns a pointer to it or
	 NULL if not found
*/
void* qremove(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp){
	if (qp == NULL) {
		return NULL;
	}
	node *current = qp->start;
	node *prev = NULL;
	while (current != NULL) {
		if (searchfn(current->data, skeyp)) {
			//condition1: when it is at the front of the queue:
			void *res = (void*)current->data; 
			if (prev == NULL){
				node *tempnode = qp->start;
			  qp->start = current->next;
				if (qp->start == NULL){
					qp->end = NULL;
				}
				free(tempnode);
				return res;
			}else{
				// other cases
				node *tempnode = current;
				prev->next = current->next;
				if (current == qp->end) {
					qp->end = prev;
				}
				free(tempnode);
				return res;
			}
		}
		prev = current;
		current = current->next;
	}
	return NULL;
}

/* concatenatenates elements of q2 into q1
	 q2 is dealocated, closed, and unusable upon completion
	 */
void qconcat(queue_t *q1p, queue_t *q2p) {
	// first move q2 to q1
	if (q1p == NULL){
		if (q2p == NULL){
			return;
		}
		free(q2p);
		return;
	}

	if (q2p == NULL) {
		return;
	}
	
	if (q1p->start == NULL) {
		if (q2p->start == NULL) {
			free(q2p);
			return;
		} else {
			q1p->start = q2p->start;
			q1p->end = q2p->end;
			free(q2p);
			return;
		}
	}

	if (q2p->start == NULL) {
		free(q2p);
		return;
	}

	q1p->end->next =q2p->start;
	q1p->end = q2p->end;
	free(q2p);
	return;
}
			
		
		

	

