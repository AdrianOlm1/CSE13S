#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "pagescanner.h"

// Variables to keep track of the HTML content and the current position
static const char *html_content;
static size_t current_position;

// Function to find the next occurrence of a substring in the HTML
static const char *find_next(const char *substring) {
    const char *result = strstr(html_content + current_position, substring);
    if (result != NULL) {
        current_position = result - html_content + 1;
    }
    return result;
}

// Function to extract the next URL from the HTML
char *pagescanner_next_url(void) {
    const char *start_tag = "<a href=\"";
    const char *end_tag = "\"";

    const char *start = find_next(start_tag);
    if (start == NULL) {
        return NULL; // No more URLs found
    }

    start += strlen(start_tag);
    const char *end = strchr(start, *end_tag);
    if (end == NULL) {
        return NULL; // Invalid URL format
    }

    size_t url_length = end - start;
    char *url = malloc(url_length + 1);
    strncpy(url, start, url_length);
    url[url_length] = '\0';

    return url;
}

// Function to initialize the scanner
void pagescanner_init( const char *html) {
    html_content = html;
    current_position = 0;

    // Sample implementation: do nothing for demonstration
}

// Function to reset the scanner
void pagescanner_reset(void) {
    current_position = 0;

    // Sample implementation: do nothing for demonstration
}
