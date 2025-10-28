#pragma once
#include "webpage.h"
#include <stddef.h>

typedef struct Index Index;

Index *index_new(void);

int index_add_posting(Index *idx, const char *word, int docid, int count);

void index_free(Index *idx);

int indexsave(const char *indexnm, const Index *idx);

Index *indexload(const char *indexnm);
