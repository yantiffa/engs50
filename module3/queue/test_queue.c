/* 
 * queue_test.c --- 
 * 
 * Author: Marco Ku
 * Created: 10-04-2025
 * Version: 1.0
 * 
 * Description: This file tests all the functions in queue.c
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "queue.h"

// Helper functions for testing
bool search_int(void *elementp, const void *keyp) {
    int *elem = (int*)elementp;
    int *key = (int*)keyp;
    return *elem == *key;
}

void print_int(void *elementp) 
{
    int *x = (int*)elementp;
    printf("%d ", *x);
}

// ---------- TEST CASES ----------

void test_qopen_qclose() 
{
    printf("Testing qopen/qclose...\n");
    queue_t *q = qopen();
    assert(q != NULL);
    qclose(q);
    printf("Passed qopen/qclose\n\n");
}

void test_qput_qget() {
    printf("Testing qput/qget...\n");
    queue_t *q = qopen();
    int a = 10, b = 20, c = 30;

    assert(qput(q, &a) == 0);
    assert(qput(q, &b) == 0);
    assert(qput(q, &c) == 0);

    int *out = (int*)qget(q);
    assert(*out == 10);

    out = (int*)qget(q);
    assert(*out == 20);

    out = (int*)qget(q);
    assert(*out == 30);

    qclose(q);
    printf("Passed qput/qget\n\n");
}

void test_qapply() {
    printf("Testing qapply...\n");
    queue_t *q = qopen();
    int a = 1, b = 2, c = 3;
    qput(q, &a);
    qput(q, &b);
    qput(q, &c);

    printf("Expected output: 1 2 3\nGot: ");
    qapply(q, print_int);
    printf("\nPassed qapply\n\n");

    qclose(q);
}

void test_qsearch() {
    printf("Testing qsearch...\n");
    queue_t *q = qopen();
    int a = 10, b = 20, c = 30;
    qput(q, &a);
    qput(q, &b);
    qput(q, &c);

    int key = 20;
    int *res = (int*)qsearch(q, search_int, &key);
    assert(res != NULL && *res == 20);
    printf("Passed qsearch\n\n");

    qclose(q);
}

void test_qremove() {
    printf("Testing qremove...\n");
    queue_t *q = qopen();
    int a = 1, b = 2, c = 3;
    qput(q, &a);
    qput(q, &b);
    qput(q, &c);

    int key = 2;
    int *res = (int*)qremove(q, search_int, &key);
    assert(res != NULL && *res == 2);

    int key2 = 2;
    res = (int*)qsearch(q, search_int, &key2);
    assert(res == NULL); // Should no longer exist

    qclose(q);
    printf("Passed qremove\n\n");
}

void test_qconcat() {
    printf("Testing qconcat...\n");
    queue_t *q1 = qopen();
    queue_t *q2 = qopen();

    int a = 1, b = 2, c = 3, d = 4;
    qput(q1, &a);
    qput(q1, &b);
    qput(q2, &c);
    qput(q2, &d);

    qconcat(q1, q2);

    printf("Expected output: 1 2 3 4\nGot: ");
    qapply(q1, print_int);
    printf("\nPassed qconcat\n\n");

    qclose(q1);
}

int main() {
    printf("===== Queue Module Tests =====\n\n");
    test_qopen_qclose();
    test_qput_qget();
    test_qapply();
    test_qsearch();
    test_qremove();
    test_qconcat();
    printf("All tests passed! \n");
    return 0;
}
