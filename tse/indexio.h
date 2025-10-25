
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

/* Load index from file. Allocates and returns an Index*, or NULL on error.
   Lines are parsed as: word followed by pairs of integers (docID count).
*/
Index *indexload(const char *indexnm);

/* Save index to file. Format:
   <word> <docID1> <count1> <docID2> <count2> ...\n
   Returns 0 on success, -1 on error.
*/
int indexsave(const char *indexnm, const Index *idx);