#define _POSIX_C_SOURCE 200809L 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>  
#include <sys/types.h>   

#define MAX_QUERY_LEN 512
#define MAX_WORDS 100

// CHANGED: tiny in-memory table for word -> count(doc 1)
typedef struct { char *w; int *cs; } entry_t;
static entry_t *gtab = NULL;         
static size_t gN = 0, gCap = 0;      

// CHANGED: helpers for table + loader
static void die(const char *m){ perror(m); exit(1); }
static void *xmalloc(size_t n){ void *p=malloc(n); if(!p) die("malloc"); return p; }
static char *xstrdup(const char *s){ size_t n=strlen(s)+1; char *p=xmalloc(n); memcpy(p,s,n); return p; }
static void add_entry(const char *w, int *c)
{
    for(size_t i=0;i<gN;i++){ if(strcmp(gtab[i].w,w)==0){ gtab[i].cs = c; return; } }
    if(gN==gCap){ gCap = gCap? gCap*2 : 256; gtab = realloc(gtab, gCap*sizeof(entry_t)); if(!gtab) die("realloc"); }
    gtab[gN].w = xstrdup(w);
    gtab[gN].cs = c;
    gN++;
}
static int get_count(const char *w, int docID){
    for(size_t i=0;i<gN;i++) if(strcmp(gtab[i].w,w)==0) return gtab[i].cs[docID];
    return 0;
}
static void lowerize(char *s){ for(;*s;++s) *s=(char)tolower((unsigned char)*s); }

// Index line format: word docID count [docID count] ...
static void load_index_all(const char *indexpath){
    FILE *f = fopen(indexpath, "r");
    if(!f){ perror("fopen index"); exit(1); }

    char buf[4096];
    while (fgets(buf, sizeof buf, f)) {
        char *tok;

        // first token = word
        tok = strtok(buf, " \t\r\n");
        if(!tok) continue;

        char word[256];
        strncpy(word, tok, sizeof(word));
        word[sizeof(word)-1] = '\0';
        lowerize(word);

        // scan pairs: docID count ...
        int *count = calloc(1000, sizeof(int));
        while (1) {
            char *d = strtok(NULL, " \t\r\n");  if(!d) break;
            char *c = strtok(NULL, " \t\r\n");  if(!c) break;
            long doc = strtol(d, NULL, 10);
            long cnt = strtol(c, NULL, 10);
            count[doc] = (int)cnt;
        }
        add_entry(word, count);
    }
    fclose(f);
}

static char* get_url(const char *pageDir, int docID) {
    char path[500];
    sprintf(path, "%s/%d", pageDir, docID);

    FILE *file = fopen(path, "r");
    if (!file) {
        return xstrdup("unknown");
    }

    char *cur = NULL;
    size_t lens = 0;
    getline(&cur, &lens, file);
    fclose(file);
    cur[strcspn(cur, "\n")] = '\0';
    return cur;
}

typedef struct {
    char *words[MAX_WORDS];
    int count;
} AndSequence;

typedef struct {
    AndSequence sequences[MAX_WORDS];
    int count;
} Query;

static bool parse_query(char *input, Query *q) {
    q->count = 0;
    char *tokens[MAX_WORDS];
    int token_count = 0;

    // Tokenize
    char *token = strtok(input, " \t\n");
    while (token != NULL && token_count < MAX_WORDS) {
        for (char *p = token; *p; p++) {
            if (!isalpha((unsigned char)*p)) {
                return false;
            }
            *p = (char)tolower((unsigned char)*p);
        }
        tokens[token_count++] = token;
        token = strtok(NULL, " \t\n");
    }

    if (token_count == 0) return true;

    // Check for invalid starting/ending
    if (strcmp(tokens[0], "and") == 0 || strcmp(tokens[0], "or") == 0) return false;
    if (strcmp(tokens[token_count-1], "and") == 0 || strcmp(tokens[token_count-1], "or") == 0) return false;

    // Parse into OR-separated AND sequences
    AndSequence *current = &q->sequences[0];
    current->count = 0;
    q->count = 1;

    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i], "or") == 0) {
            if (current->count == 0) return false;
            if (i + 1 < token_count && (strcmp(tokens[i+1], "and") == 0 || strcmp(tokens[i+1], "or") == 0)) return false;
            q->count++;
            current = &q->sequences[q->count - 1];
            current->count = 0;
        } else if (strcmp(tokens[i], "and") == 0) {
            if (current->count == 0) return false;
            if (i + 1 < token_count && (strcmp(tokens[i+1], "and") == 0 || strcmp(tokens[i+1], "or") == 0)) return false;
        } else {
            if (strlen(tokens[i]) < 3) continue;
            current->words[current->count++] = tokens[i];
        }
    }

    return (q->sequences[q->count - 1].count > 0);
}

static void process_query(char* query, const char *pageDir) {
    Query q;
    if (!parse_query(query, &q)) {
        printf("[invalid query]\n");
        return;
    }

    if (q.count == 0 || q.sequences[0].count == 0) return;

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

    // Compute scores
    for (int docID = 1; docID < 1000; docID++) {
        int total_score = 0;
        for (int s = 0; s < q.count; s++) {
            int min_score = -1;
            bool has_all = true;
            for (int w = 0; w < q.sequences[s].count; w++) {
                int c = get_count(q.sequences[s].words[w], docID);
                if (c == 0) { has_all = false; break; }
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
        fprintf(stderr, "No document match\n");
    }
}

int main(int argc, char **argv) {
    if (argc != 3){
        fprintf(stderr, "wrong number of commands\n");
        return 1;
    }
    char *pageDir = argv[1];
    char *indexfile = argv[2];
    char query[MAX_QUERY_LEN];
    load_index_all(indexfile);

    while (true) {
        printf("> ");
        fflush(stdout);

        if (fgets(query, sizeof(query), stdin) == NULL) {
            printf("\n");
            break;
        }

        process_query(query, pageDir);
    }

    return 0;
}
