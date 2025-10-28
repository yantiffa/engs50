#pragma once
#include <stddef.h>

typedef struct Posting {
    int docid;
    int count;
    struct Posting *next;
} Posting;

typedef struct Entry {
    char *word;
    Posting *postings;
    struct Entry *next;
} Entry;

typedef struct Index {
    Entry *head;
} Index;

Index *index_new(void);

int index_add_posting(Index *idx, const char *word, int docid, int count);

void index_free(Index *idx);

int indexsave(const char *indexnm, const Index *idx);

Index *indexload(const char *indexnm);
