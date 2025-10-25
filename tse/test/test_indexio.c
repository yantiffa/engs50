// /workspaces/modules/tse/test/test_indexio.c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "indexio.h"

/* Opaque forward declaration for Index and prototypes from ../utils/indexio.c */
typedef struct Index Index;
Index *index_new(void);
void index_free(Index *idx);
int index_add_posting(Index *idx, const char *word, int docid, int count);
int indexsave(const char *indexnm, const Index *idx);
Index *indexload(const char *indexnm);

/* Helpers */
static char *read_file_to_string(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return NULL; }
    long sz = ftell(f);
    if (sz < 0) { fclose(f); return NULL; }
    rewind(f);
    char *buf = malloc((size_t)sz + 1);
    if (!buf) { fclose(f); return NULL; }
    size_t r = fread(buf, 1, (size_t)sz, f);
    buf[r] = '\0';
    fclose(f);
    return buf;
}

static char *create_temp_with_content(const char *content) {
    char template[] = "/tmp/test_indexXXXXXX";
    int fd = mkstemp(template);
    if (fd < 0) return NULL;
    FILE *f = fdopen(fd, "w");
    if (!f) { close(fd); unlink(template); return NULL; }
    if (content && fputs(content, f) == EOF) { fclose(f); unlink(template); return NULL; }
    fclose(f); /* closes fd as well */
    return strdup(template);
}

int main(void) {
    /* Test 1: save -> load -> save roundtrip consistency */
    Index *idx = index_new();
    assert(idx != NULL);
    assert(index_add_posting(idx, "apple", 1, 2) == 0);
    assert(index_add_posting(idx, "apple", 3, 1) == 0);
    assert(index_add_posting(idx, "banana", 2, 5) == 0);

    char *f1 = create_temp_with_content(NULL);
    assert(f1 != NULL);
    assert(indexsave(f1, idx) == 0);

    Index *loaded = indexload(f1);
    assert(loaded != NULL);

    char *f2 = create_temp_with_content(NULL);
    assert(f2 != NULL);
    assert(indexsave(f2, loaded) == 0);

    char *s1 = read_file_to_string(f1);
    char *s2 = read_file_to_string(f2);
    assert(s1 != NULL && s2 != NULL);
    assert(strcmp(s1, s2) == 0);

    /* cleanup test1 */
    free(s1); free(s2);
    unlink(f1); unlink(f2);
    free(f1); free(f2);
    index_free(idx);
    index_free(loaded);

    printf("Test 1: PASSED (roundtrip save/load consistency)\n");

    /* Test 2: loading file with invalid words/tokens skips appropriately */
    const char *content =
        "apple 1 2 3 1\n"
        "Abc 1 1\n"
        "pear 2 1 999 9\n"
        "invalid_word 1 2\n"
        "orange 5 x\n"; /* last line has invalid number token */
    char *f3 = create_temp_with_content(content);
    assert(f3 != NULL);

    Index *idx2 = indexload(f3);
    assert(idx2 != NULL);

    char *f4 = create_temp_with_content(NULL);
    assert(f4 != NULL);
    assert(indexsave(f4, idx2) == 0);

    char *out = read_file_to_string(f4);
    assert(out != NULL);

    /* Expect 'apple' and 'pear' present, 'Abc' and 'invalid_word' absent.
       'orange' line had invalid number and should be skipped (no valid posting). */
    assert(strstr(out, "apple") != NULL);
    assert(strstr(out, "pear") != NULL);
    assert(strstr(out, "Abc") == NULL);
    assert(strstr(out, "invalid_word") == NULL);
    /* 'orange' should not appear because its posting was invalid */
    assert(strstr(out, "orange") == NULL);

    printf("Test 2: PASSED (invalid lines/tokens skipped appropriately)\n");

    /* cleanup test2 */
    unlink(f3); unlink(f4);
    free(f3); free(f4);
    free(out);
    index_free(idx2);

    /* Test 3: index_add_posting rejects bad args */
    assert(index_add_posting(NULL, "a", 1, 1) == -1);
    Index *idx3 = index_new();
    assert(idx3 != NULL);
    assert(index_add_posting(idx3, NULL, 1, 1) == -1);
    assert(index_add_posting(idx3, "a", 0, 1) == -1);
    assert(index_add_posting(idx3, "a", 1, 0) == -1);
    index_free(idx3);

    printf("Test 3: PASSED (argument validation for index_add_posting)\n");

    printf("ALL TESTS PASSED\n");
    return 0;
}