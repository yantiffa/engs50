#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_QUERY_LEN 512
#define MAX_WORDS 100

// Function to process a single line of query input
static void process_query(char* query);

int main() {
    char query[MAX_QUERY_LEN];

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
        process_query(query);
    }

    return 0;
}

// Processes a single line of query input.
// Normalizes valid words to lowercase and prints them.
// Rejects queries with non-alphabetic characters or empty queries.
static void process_query(char* query) {
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
            *p = tolower((unsigned char)*p);
        }

        if (!is_valid) {
            break;
        }

        words[word_count++] = token;
        token = strtok(NULL, " \t\n");
    }

    // Print results based on validation
    if (!is_valid) {
        printf("[invalid query]\n");
    } else if (word_count > 0) {
        // Print the normalized words
        for (int i = 0; i < word_count; i++) {
            printf("%s%s", words[i], (i == word_count - 1) ? "" : " ");
        }
        printf("\n");
    }
    // If word_count is 0 (empty line), do nothing.
}