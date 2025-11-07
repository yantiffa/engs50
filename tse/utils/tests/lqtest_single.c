#include <stdio.h>
#include <stdlib.h>
#include "lqueue.h"

typedef struct {
    int id;
} item_t;

void print_item(void *ep) {
    item_t *ip = ep;
    printf("%d ", ip->id);
}

int main(void) {
    lqueue_t *lq = lqopen();
    if (lq == NULL) {
        fprintf(stderr, "lqopen failed\n");
        return 1;
    }

    // put 3 items
    for (int i = 1; i <= 3; i++) {
        item_t *it = malloc(sizeof(item_t));
        it->id = i;
        if (lqput(lq, it) != 0) {
            fprintf(stderr, "lqput failed on %d\n", i);
        }
    }

    // apply
    printf("Queue contents (apply): ");
    lqapply(lq, print_item);
    printf("\n");

    // get items back
    for (int i = 1; i <= 3; i++) {
        item_t *it = lqget(lq);
        if (!it) {
            printf("Got NULL at i=%d (ERROR)\n", i);
        } else {
            printf("Got item: %d\n", it->id);
            free(it);
        }
    }

    // now queue should be empty
    if (lqget(lq) != NULL) {
        printf("Expected empty queue but got non-NULL (ERROR)\n");
    } else {
        printf("Queue empty as expected\n");
    }

    lqclose(lq);
    return 0;
}
