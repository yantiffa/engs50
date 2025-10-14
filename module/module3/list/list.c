/* 
 * list.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 10-01-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include "list.h"
#include <string.h>

// keep track of the head
static car_t* head = NULL;

int32_t lput(car_t *cp) {
	if (cp == NULL) {
		return -1;
	}
  cp->next = head;
	head = cp;
	return 0;
}

car_t *lget() {
	if (head == NULL) {
		return NULL;
	}
	car_t* target = head;
	head = head->next;
	target->next = NULL;
	return target;
}

void lapply(void (*fn)(car_t *cp)) {
	car_t* curr = head;
	while (curr != NULL) {
		fn(curr);
		curr = curr->next;
	}
}

car_t *lremove(char *platep) {
	car_t* curr = head;
	car_t* prev = NULL;

	while (curr != NULL) {
		if (!strcmp(curr->plate, platep)) {
				car_t* temp = curr;
				if (prev == NULL) {
					head = curr->next;
				}else {
					prev->next = curr->next;
				}
				return temp;
			}
		else {
			car_t* temp = curr;
			curr = curr->next;
			prev = temp;
		}
	}
	return NULL;
}
			
	
		
