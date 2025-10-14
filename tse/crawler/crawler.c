#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../webpage.h"

// Ensure the definition of struct webpage is available
// If webpage.h does not define struct webpage, add its definition here:
typedef struct webpage {
  char *url;                               // url of the page
  char *html;                              // html code of the page
  size_t html_len;                         // length of html code
  int depth;                               // depth of crawl
} webpage_t;

int main() {

    webpage_t seed = *webpage_new("https://thayer.github.io/engs50/", 0, NULL);

    if(!webpage_fetch(&seed)) {
        fprintf(stderr, "Failed to fetch webpage\n");
        exit(EXIT_FAILURE);
    }
    
    char *word = malloc(100 * sizeof(char));
    webpage_getNextWord(&seed, 0, &word);
    printf("First word: %s\n", word);

    free(word);
    printf("hello hi\n");

    return 0;
}