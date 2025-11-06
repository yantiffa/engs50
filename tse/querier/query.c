#define _POSIX_C_SOURCE 200809L 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>

#define MAX_QUERY_LEN 512
#define MAX_WORDS 100
#define MAX_DOCS 1000

/* ===== In-memory index table: word -> counts[docID] ===== */

typedef struct { 
    char *w;      // word
    int  *cs;     // counts per docID (size MAX_DOCS)
} entry_t;

static entry_t *gtab = NULL;         
static size_t gN = 0, gCap = 0;      

/* ===== Utility helpers ===== */

static void die(const char *m){ 
    perror(m); 
    exit(1); 
}

static void *xmalloc(size_t n){ 
    void *p = malloc(n); 
    if (!p) die("malloc"); 
    return p; 
}

static char *xstrdup(const char *s){ 
    size_t n = strlen(s) + 1; 
    char *p = xmalloc(n); 
    memcpy(p, s, n); 
    return p; 
}

static void lowerize(char *s){ 
    for (; *s; ++s) {
        *s = (char)tolower((unsigned char)*s);
    }
}

/* ===== Index loading + lookup ===== */

static void add_entry(const char *w, int *c)
{
    // if word already present, replace counts
    for (size_t i = 0; i < gN; i++) {
        if (strcmp(gtab[i].w, w) == 0) {
            free(gtab[i].cs);
            gtab[i].cs = c;
            return;
        }
    }

    if (gN == gCap) {
        gCap = gCap ? gCap * 2 : 256;
        gtab = realloc(gtab, gCap * sizeof(entry_t));
        if (!gtab) die("realloc");
    }

    gtab[gN].w  = xstrdup(w);
    gtab[gN].cs = c;
    gN++;
}

static int get_count(const char *w, int docID){
    if (docID < 0 || docID >= MAX_DOCS) return 0;
    for (size_t i = 0; i < gN; i++) {
        if (strcmp(gtab[i].w, w) == 0) {
            return gtab[i].cs[docID];
        }
    }
    return 0;
}

// Index line format: word docID count [docID count] ...
static void load_index_all(const char *indexpath){
    FILE *f = fopen(indexpath, "r");
    if (!f) { 
        perror("fopen index"); 
        exit(1); 
    }

    char buf[4096];
    while (fgets(buf, sizeof buf, f)) {
        char *tok = strtok(buf, " \t\r\n");
        if (!tok) continue;

        char word[256];
        strncpy(word, tok, sizeof(word));
        word[sizeof(word) - 1] = '\0';
        lowerize(word);

        int *counts = calloc(MAX_DOCS, sizeof(int));
        if (!counts) die("calloc");

        while (1) {
            char *d = strtok(NULL, " \t\r\n");
            if (!d) break;
            char *c = strtok(NULL, " \t\r\n");
            if (!c) break;

            long doc = strtol(d, NULL, 10);
            long cnt = strtol(c, NULL, 10);
            if (doc >= 0 && doc < MAX_DOCS) {
                counts[doc] = (int)cnt;
            }
        }

        add_entry(word, counts);
    }

    fclose(f);
}

/* ===== Read URL from crawler page file ===== */

static char* get_url(const char *pageDir, int docID) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%d", pageDir, docID);

    FILE *file = fopen(path, "r");
    if (!file) {
        return xstrdup("unknown");
    }

    char *cur = NULL;
    size_t lens = 0;
    if (getline(&cur, &lens, file) == -1) {
        fclose(file);
        free(cur);
        return xstrdup("unknown");
    }
    fclose(file);

    cur[strcspn(cur, "\n")] = '\0';
    return cur;    // caller must free
}

/* ===== Query parsing ===== */

typedef struct {
    char *words[MAX_WORDS];
    int   count;
} AndSequence;

typedef struct {
    AndSequence sequences[MAX_WORDS]; // OR of ANDs
    int         count;
} Query;

static bool parse_query(char *input, Query *q) {
    q->count = 0;
    char *tokens[MAX_WORDS];
    int token_count = 0;

    // Tokenize by whitespace
    char *token = strtok(input, " \t\n");
    while (token != NULL && token_count < MAX_WORDS) {
        for (char *p = token; *p; p++) {
            if (!isalpha((unsigned char)*p)) {
                return false;   // invalid char
            }
            *p = (char)tolower((unsigned char)*p);
        }
        tokens[token_count++] = token;
        token = strtok(NULL, " \t\n");
    }

    if (token_count == 0) {
        // empty line is fine, just no query to run
        return true;
    }

    // cannot start or end with and/or
    if (strcmp(tokens[0], "and") == 0 || strcmp(tokens[0], "or") == 0) return false;
    if (strcmp(tokens[token_count - 1], "and") == 0 || strcmp(tokens[token_count - 1], "or") == 0) return false;

    // Parse into OR-separated AND sequences
    AndSequence *current = &q->sequences[0];
    current->count = 0;
    q->count = 1;

    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i], "or") == 0) {
            // end current AND group, start new one
            if (current->count == 0) return false;
            if (i + 1 < token_count &&
                (strcmp(tokens[i + 1], "and") == 0 || strcmp(tokens[i + 1], "or") == 0)) {
                return false;
            }
            q->count++;
            current = &q->sequences[q->count - 1];
            current->count = 0;
        } else if (strcmp(tokens[i], "and") == 0) {
            // just a separator inside AND group
            if (current->count == 0) return false;
            if (i + 1 < token_count &&
                (strcmp(tokens[i + 1], "and") == 0 || strcmp(tokens[i + 1], "or") == 0)) {
                return false;
            }
        } else {
            // normal word
            if (strlen(tokens[i]) < 3) continue;  // ignore short words
            current->words[current->count++] = tokens[i];
        }
    }

    // at least one word in the last sequence
    return (q->sequences[q->count - 1].count > 0);
}


/* ===== Query processing ===== */
int res = 0;
static void process_query(char* query, const char *pageDir) {
    Query q;
    if (!parse_query(query, &q)) {
        printf("[invalid query]\n");
        return;
    }

    if (q.count == 0 || q.sequences[0].count == 0) {
        // empty or all-too-short words: ignore
        return;
    }
    
    // Print normalized query
    bool first = true;
    for (int s = 0; s < q.count; s++) {
        for (int w = 0; w < q.sequences[s].count; w++) {
            if (!first) printf(" ");
            printf("%s", q.sequences[s].words[w]);
            first = false;
        }
        if (s < q.count - 1) printf(" or ");
    }
    printf("\n");

    int res = 0;

    // Compute scores over all docs
    for (int docID = 1; docID < MAX_DOCS; docID++) {
        int total_score = 0;

        for (int s = 0; s < q.count; s++) {
            int  min_score = -1;
            bool has_all   = true;

            for (int w = 0; w < q.sequences[s].count; w++) {
                int c = get_count(q.sequences[s].words[w], docID);
                if (c == 0) { 
                    has_all = false; 
                    break; 
                }
                if (min_score < 0 || c < min_score) min_score = c;
            }

            if (has_all) {
                total_score += min_score;
            }
        }

        if (total_score > 0) {
            char *url = get_url(pageDir, docID);
            printf("rank: %d: doc: %d: %s\n", total_score, docID, url);
            free(url);
            res++;
        }
    }
    if (res == 0) {
        printf("No documents match.\n");
    }
}


static bool file_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

//  0  = directory exists but is NOT crawled (no file "1")
// -1  = directory exists but is NOT accessible (permission denied)
// -2  = directory does not exist or is not a directory
static int crawler_dir_status(const char *pageDir) {
    // first: does the directory exist and is it a dir?
    struct stat st;
    if (stat(pageDir, &st) != 0 || !S_ISDIR(st.st_mode)) {
        return -2;  // doesn't exist or not a directory
    }

    // now check for pageDir/1
    char path[512];
    snprintf(path, sizeof(path), "%s/1", pageDir);

    if (stat(path, &st) != 0) {
        if (errno == EACCES) {
            return -1;  // we don't have permission to look at it
        }
        return 0;      // no "1" file -> not crawled
    }

    if (!S_ISREG(st.st_mode)) {
        return 0;      // "1" exists but isn't a normal file -> treat as not crawled
    }

    return 1;          // looks like a crawled directory
}

/* ===== Cleanup for valgrind ===== */

static void free_index(void) {
    for (size_t i = 0; i < gN; i++) {
        free(gtab[i].w);
        free(gtab[i].cs);
    }
    free(gtab);
    gtab  = NULL;
    gN    = 0;
    gCap  = 0;
}


int main(int argc, char **argv) {
    bool quiet = false;

    // usage: query <pageDirectory> <indexFile> [-q]
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "usage: %s <pageDirectory> <indexFile> [-q]\n", argv[0]);
        return 1;
    }

    char *pageDir   = argv[1];
    char *indexfile = argv[2];

    if (argc == 4) {
        if (strcmp(argv[3], "-q") != 0) {
            fprintf(stderr, "usage: %s <pageDirectory> <indexFile> [-q]\n", argv[0]);
            return 1;
        }
        quiet = true;
    }

    int status = crawler_dir_status(pageDir);
    if (status == -2) {
        fprintf(stderr, "error: pageDirectory '%s' does not exist or is not a directory\n", pageDir);
        return 1;
    } else if (status == -1) {
        fprintf(stderr, "error: pageDirectory '%s' is not accessible\n", pageDir);
        return 1;
    } else if (status == 0) {
        fprintf(stderr, "error: pageDirectory '%s' has not been crawled\n", pageDir);
        return 1;
    }

    // index file exists (readability checked by fopen inside load_index_all)
    if (!file_exists(indexfile)) {
        fprintf(stderr, "error: indexFile '%s' does not exist or is not a regular file\n", indexfile);
        return 1;
    }

    // load index into memory
    load_index_all(indexfile);

    char query[MAX_QUERY_LEN];

    while (true) {
        if (!quiet) {
            printf("> ");
            fflush(stdout);
        }

        if (fgets(query, sizeof(query), stdin) == NULL) {
            if (!quiet) printf("\n");
            break;
        }

        process_query(query, pageDir);
    }

    free_index();  
    return 0;
}
