/* 
 * listtest.c --- 
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
#include <stdlib.h>
#include <string.h>

static void deflation(car_t* car) {
	car->price = car->price/2;
}

int main(){
	// generate some car_t samples
	 car_t* car1 = malloc(sizeof(car_t));
	 strcpy(car1->plate, "ABCD1234");
	 car1->price = 100000;
	 car1->year = 2024;

	 car_t* car2 = malloc(sizeof(car_t));
	 strcpy(car2->plate, "HH10086");
	 car2->price = 10000;
	 car2->year = 2005;

	 // test for lput() to an empty list
	 if (lput(car1) != 0) {
		 fprintf(stderr, "the function lput() was unsuccessful when putted to an empty list.\n");
		 exit(EXIT_FAILURE);
	 }
	 printf("success: lput() to an empty list\n");

	 // test for lput to an non-empty list
	 if (lput(car2) != 0) {
		 fprintf(stderr, "the function lput() was unsuccessful when putted to a non-empty list.\n");
		 exit(EXIT_FAILURE);
	 }

	 //get() from a not-empty list
	 car_t *node = lget();
	 if (node == NULL) {
		 fprintf(stderr, "the function lget() was unsuccessful from an non-empty list. Expected a car info, returned NULL instead.\n");
		 exit(EXIT_FAILURE);
	 }
	 if (node->price != 10000){
		  fprintf(stderr, "the function lget() was unsuccessful from an non-empty list. Car's info is incorrect.\n");
			exit(EXIT_FAILURE);
	 }
	 
		
	 free(node);
	 node = lget();
	 free(node);
	 printf("success: get() from a not-empty list\n");
	 node = lget();

	 if (node != NULL) {
		 fprintf(stderr, "the function lget() was unsuccessful from an empty list. Expected NULL, returned non-NULL instead.\n");
		 exit(EXIT_FAILURE);
	 }
	 printf("success: get() from a empty list\n");
	 //apply() a function to an empty list
	 lapply(deflation);
	 if (lget() != NULL) {
		 fprintf(stderr, "the function lapply() failed; list should remain empty.\n");
		 exit(EXIT_FAILURE);
	 }
	 printf("success: apply() a function to an empty list\n");
	 car_t* car3 = malloc(sizeof(car_t));
	 strcpy(car3->plate, "ABCD1234");
	 car3->price = 100000;
	 car3->year = 2024;

	 car_t* car4 = malloc(sizeof(car_t));
	 strcpy(car4->plate, "HH10086");
	 car4->price = 10000;
	 car4->year = 2005;
	 
	 lput(car3);
	 lput(car4);
	 lapply(deflation);
	 node = lget();
	 if (node->price != 5000){
		 fprintf(stderr, "the function lapply() failed; function did not do the right thing.\n");
		 exit(EXIT_FAILURE); 
	 }
	 free(node);
	 node = lget();
	 if (node->price != 50000){
		 fprintf(stderr, "the function lapply() failed; function did not do the right thing.\n");
		 exit(EXIT_FAILURE); 
	 }
	 free(node);
	 printf("success: apply() a function to a non-empty list\n");
	 //remove() from an empty list
	 //remove() from a non-empty list at the end
	 //remove() from a non-empty list at the beginning
	 //remove() from a non-empty list somewhere in the middle
	 printf("All test cases passed!\n");
	 exit(EXIT_SUCCESS); 
}
