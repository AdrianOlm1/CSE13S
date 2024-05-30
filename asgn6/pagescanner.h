#ifndef PAGESCANNER_H
#define PAGESCANNER_H

/**
 * Initializes the page scanner with the given URL and HTML content.
 * This function should be called before using other page scanner functions.
 */
void pagescanner_init(const char *html);

/**
 * Extracts the next URL from the HTML content.
 * Returns the next URL as a dynamically allocated string, or NULL if no more URLs are found.
 * The caller is responsible for freeing the returned string.
 */
char *pagescanner_next_url(void);

/**
 * Resets the page scanner to its initial state.
 * This function should be called when starting to scan a new HTML content.
 */
void pagescanner_reset(void);

#endif // PAGESCANNER_H
