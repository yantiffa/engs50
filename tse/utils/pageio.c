#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "webpage.h"


int32_t pagesave(webpage_t *pagep, int id, char *dirname) {
	char path[256];
	sprintf(path, "%s/%d", dirname, id);
	FILE *fp = fopen(path, "w");

	if (fp==NULL) {
		//error
		return -1;
	}
	fprintf(fp, "%s\n", webpage_getURL(pagep));
	fprintf(fp, "%d\n", webpage_getDepth(pagep));
	fprintf(fp, "%d\n", webpage_getHTMLlen(pagep));
	fprintf(fp, "%s\n", webpage_getHTML(pagep));
	
	fclose(fp);
	return 0;
}

webpage_t *pageload(int id, char *dirname) {
    char path[256];
    sprintf(path, "%s/%d", dirname, id);
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        // error
        return NULL;
    }

    char *url = NULL;
    int depth = -1;
    int html_len = -1;
    char *html = NULL;

    // Read the webpage data from the file
    {
        size_t urlcap = 0;
        ssize_t urllen = getline(&url, &urlcap, fp);
        if (urllen == -1) {
            /* failed to read URL line */
            free(url);
            fclose(fp);
            return NULL;
        }
        if (urllen > 0 && url[urllen - 1] == '\n') {
            url[urllen - 1] = '\0';
            /* urllen--; not needed further */
        }
    }
    if (fscanf(fp, "%d\n", &depth) != 1) {
        free(url);
        fclose(fp);
        return NULL;
    }
    if (fscanf(fp, "%d\n", &html_len) != 1) {
        free(url);
        fclose(fp);
        return NULL;
    }
    html = malloc((html_len > 0 ? html_len : 0) + 1);
    if (html != NULL) {
        int i = 0;
        for (; i < html_len; ++i) {
            int c = fgetc(fp);
            if (c == EOF) {
                break;
            }
            html[i] = (char)c;
        }
        html[i] = '\0';
    }

    fclose(fp);

    // Create a new webpage_t object and return it
    webpage_t *page = webpage_new(url, depth, html);
    return page;
}
