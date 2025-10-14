/* 
 * testlist.c --- 
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
         free(car1);
         exit(EXIT_FAILURE);
     }
     /* ownership transferred to list */
     car1 = NULL;
     printf("success: lput() to an empty list\n");

     // test for lput to an non-empty list
     if (lput(car2) != 0) {
         fprintf(stderr, "the function lput() was unsuccessful when putted to a non-empty list.\n");
         free(car2);
         /* make sure we free any items already in the list */
         car_t *tmp;
         while ((tmp = lget()) != NULL) free(tmp);
         exit(EXIT_FAILURE);
     }
     car2 = NULL;
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
     
     if (lput(car3) != 0) {
         fprintf(stderr, "the function lput() was unsuccessful when putted to list.\n");
         free(car3);
         car_t *tmp;
         while ((tmp = lget()) != NULL) free(tmp);
         exit(EXIT_FAILURE);
     }
     car3 = NULL;

     if (lput(car4) != 0) {
         fprintf(stderr, "the function lput() was unsuccessful when putted to list.\n");
         free(car4);
         car_t *tmp;
         while ((tmp = lget()) != NULL) free(tmp);
         exit(EXIT_FAILURE);
     }
     car4 = NULL;

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
    car_t *removed = lremove("some_plate");
    if (removed == NULL)
    {
        printf("success: remove() from an empty list\n");
    }
    //create 5 cars
    car_t* car5 = malloc(sizeof(car_t));
    strcpy(car5->plate, "QQQ0000");

    car_t* car6 = malloc(sizeof(car_t));
    strcpy(car6->plate, "QWE1234");

    car_t* car7 = malloc(sizeof(car_t));
    strcpy(car7->plate, "ASD2345");

    car_t* car8 = malloc(sizeof(car_t));
    strcpy(car8->plate, "ZXC3456");

    car_t* car9 = malloc(sizeof(car_t));
    strcpy(car9->plate, "CCC4567");

    if (lput(car5) != 0) { free(car5); car_t *tmp; while ((tmp = lget()) != NULL) free(tmp); exit(EXIT_FAILURE); }
    car5 = NULL;
    if (lput(car6) != 0) { free(car6); car_t *tmp; while ((tmp = lget()) != NULL) free(tmp); exit(EXIT_FAILURE); }
    car6 = NULL;
    if (lput(car7) != 0) { free(car7); car_t *tmp; while ((tmp = lget()) != NULL) free(tmp); exit(EXIT_FAILURE); }
    car7 = NULL;
    if (lput(car8) != 0) { free(car8); car_t *tmp; while ((tmp = lget()) != NULL) free(tmp); exit(EXIT_FAILURE); }
    car8 = NULL;
    if (lput(car9) != 0) { free(car9); car_t *tmp; while ((tmp = lget()) != NULL) free(tmp); exit(EXIT_FAILURE); }
    car9 = NULL;

    //remove() from a non-empty list at the end
    removed = lremove("QQQ0000");
    if (removed != NULL)
    {
        printf("success: remove() from a non-empty llist at the end. Car just removed: %s\n", removed->plate);
        free(removed);
    }
    else
    {
        printf("Car not found\n");
    }

    //remove() from a non-empty list at the beginning
    removed = lremove("CCC4567");
    if (removed != NULL)
    {
        printf("success: remove() from a non-empty list at the beginning. Car just removed: %s\n", removed->plate);
        free(removed);
    }
    else
    {
        printf("Car not found\n");
    }
    //remove() from a non-empty list somewhere in the middle

    removed = lremove("ASD2345");
    if (removed != NULL)
    {
        printf("success: remove() from a non-empty list somewhere in the middle. Car just removed: %s\n", removed->plate);
        free(removed);
    }
    else
    {
        printf("Car not found\n");
    }

    /* cleanup: free any remaining nodes still in the list to avoid leaks */
    while ((node = lget()) != NULL) {
        free(node);
    }

    printf("All test cases passed!\n");
    exit(EXIT_SUCCESS); 
}
