// /workspaces/modules/tse/utils/indexio.c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "indexio.h"


/* Create and destroy index */
Index *index_new(void) {
    Index *idx = malloc(sizeof(Index));
    if (!idx) return NULL;
    idx->head = NULL;
    return idx;
}

static void posting_free_list(Posting *p) {
    while (p) {
        Posting *n = p->next;
        free(p);
        p = n;
    }
}

static void entry_free_list(Entry *e) {
    while (e) {
        Entry *n = e->next;
        free(e->word);
        posting_free_list(e->postings);
        free(e);
        e = n;
    }
}

void index_free(Index *idx) {
    if (!idx) return;
    entry_free_list(idx->head);
    free(idx);
}

/* Add a posting to index; creates entry if necessary. Returns 0 on success, -1 on error. */
int index_add_posting(Index *idx, const char *word, int docid, int count) {
    if (!idx || !word || docid <= 0 || count <= 0) return -1;

    Entry *e = idx->head;
    Entry *prev = NULL;
    while (e) {
        if (strcmp(e->word, word) == 0) break;
        prev = e;
        e = e->next;
    }

    if (!e) {
        e = malloc(sizeof(Entry));
        if (!e) return -1;
        e->word = strdup(word);
        if (!e->word) { free(e); return -1; }
        e->postings = NULL;
        e->next = NULL;
        if (prev) prev->next = e; else idx->head = e;
    }

    Posting *p = malloc(sizeof(Posting));
    if (!p) return -1;
    p->docid = docid;
    p->count = count;
    p->next = NULL;

    if (!e->postings) {
        e->postings = p;
    } else {
        Posting *q = e->postings;
        while (q->next) q = q->next;
        q->next = p;
    }
    return 0;
}

/* Save index to file. Format:
   <word> <docID1> <count1> <docID2> <count2> ...\n
   Returns 0 on success, -1 on error.
*/
int indexsave(const char *indexnm, const Index *idx) {
    if (!indexnm || !idx) return -1;
    FILE *f = fopen(indexnm, "w");
    if (!f) return -1;

    for (Entry *e = idx->head; e; e = e->next) {
        if (fprintf(f, "%s", e->word) < 0) { fclose(f); return -1; }
        for (Posting *p = e->postings; p; p = p->next) {
            if (fprintf(f, " %d %d", p->docid, p->count) < 0) { fclose(f); return -1; }
        }
        if (fprintf(f, "\n") < 0) { fclose(f); return -1; }
    }

    if (fclose(f) != 0) return -1;
    return 0;
}

/* Load index from file. Allocates and returns an Index*, or NULL on error.
   Lines are parsed as: word followed by pairs of integers (docID count).
*/
Index *indexload(const char *indexnm) {
    if (!indexnm) return NULL;
    FILE *f = fopen(indexnm, "r");
    if (!f) return NULL;

    Index *idx = index_new();
    if (!idx) { fclose(f); return NULL; }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, f)) != -1) {
        char *saveptr = NULL;
        char *tok = strtok_r(line, " \t\r\n", &saveptr);
        if (!tok) continue; /* empty line */

        /* validate word: only lowercase letters (per spec) */
        int ok = 1;
        for (char *c = tok; *c; ++c) {
            if (!('a' <= *c && *c <= 'z')) { ok = 0; break; }
        }
        if (!ok) continue; /* skip invalid word */

        char *word = tok;
        while (1) {
            char *d_tok = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!d_tok) break; /* no more tokens */
            char *c_tok = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!c_tok) break; /* unmatched doc/count, ignore trailing */

            errno = 0;
            long docid = strtol(d_tok, NULL, 10);
            if (errno || docid <= 0) continue;
            errno = 0;
            long count = strtol(c_tok, NULL, 10);
            if (errno || count <= 0) continue;

            if (index_add_posting(idx, word, (int)docid, (int)count) != 0) {
                free(line);
                fclose(f);
                index_free(idx);
                return NULL;
            }
        }
    }

    free(line);
    fclose(f);
    return idx;
}