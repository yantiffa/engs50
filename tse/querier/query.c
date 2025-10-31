#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define _POSIX_C_SOURCE 200809L   
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
static bool is_stop_or_short(const char *w){
    size_t L = strlen(w);
    if(L < 3) return true;
    return (strcmp(w,"and")==0 || strcmp(w,"or")==0);
}


// Index line format: word docID count [docID count] ...
static void load_index_all(const char *indexpath){
    FILE *f = fopen(indexpath, "r");
    if(!f){ perror("fopen index"); exit(1); }

    char buf[4096];
    while (fgets(buf, sizeof buf, f)) {
        // CHANGED: removed unused 'save'
        char *tok;

        // first token = word
        tok = strtok(buf, " \t\r\n");
        if(!tok) continue;

        char word[256];
        strncpy(word, tok, sizeof(word));
        word[sizeof(word)-1] = '\0';
        lowerize(word);   // CHANGED: use your helper so it's not "unused"

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
    cur[strcspn(cur, "\n")] = "\0";
    return cur;
}

// Function to process a single line of query input
static void process_query(char* query, const char *pageDir) {
    char* words[MAX_WORDS];
    int word_count = 0;
    bool is_valid = true;

    // Tokenize the query string by spaces, tabs, and newlines
    char* token = strtok(query, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        // Validate and normalize the token
        for (char* p = token; *p; p++) {
            if (!isalpha((unsigned char)*p)) {
                is_valid = false;
                break;
            }
            *p = (char)tolower((unsigned char)*p);
        }
        if (!is_valid) break;

        // CHANGED: keep only words >=3 chars and not and/or
        if (!is_stop_or_short(token)) {
            words[word_count++] = token;
        }

        token = strtok(NULL, " \t\n");
    }

    // Print results based on validation
    if (!is_valid) {
        printf("[invalid query]\n");
    } else if (word_count > 0) {
        for (int i = 0; i < word_count; i++) {
            if (i) printf(" ");
            printf("%s", words[i]);
        }
        printf("\n");

        for (int docID = 1; docID<1000; docID++) {
            int min = -1;
            bool has_all = true;
            for (int i = 0; i < word_count; i++) {
                int c = get_count(words[i], docID);
                if (c == 0) { has_all = false; break; }
                if (min < 0 || c < min) min = c;
            }
            if (has_all) {
                char *url = get_url(pageDir, docID);
                printf("rank: %d: doc: %d: %s\n", min, docID, url);
                free(url);
            }
        }
    }
}



// CHANGED: accept args; last arg is treated as index file (so both ./q idx and ./q pagedir idx work)
int main(int argc, char **argv) {  // CHANGED
    if (argc != 3){
        fprintf(stderr, "wrong number of commands\n");
        return 1;
    }
    char *pageDir = argv[1];
    char *indexfile = argv[2];
    char query[MAX_QUERY_LEN];
    load_index_all(indexfile);   // CHANGED: load the index (doc 1) once

    // Loop indefinitely, prompting for and processing queries
    while (true) {
        printf("> ");
        fflush(stdout);

        // Read a line from stdin
        if (fgets(query, sizeof(query), stdin) == NULL) {
            // End-of-file (Ctrl-D) detected
            printf("\n");
            break;
        }

        // Process the read line
        process_query(query, pageDir);
    }

    return 0;
}
