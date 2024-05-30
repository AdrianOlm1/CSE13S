#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "crawler.h"
#include "curl.h"
#include "pagedir.h"
#include "pagescanner.h"
#include "url.h"
#include "hashtable.h"

// Function to extract URLs from a webpage and add them to the bag
static void extract_and_add_links(webpage_t *page, bag_t *bag, hashtable_t *url_set) {
    char *url;
    int depth = page->depth + 1;

    pagescanner_init(page->html);

    while ((url = pagescanner_next_url()) != NULL) {
        char *normalized_url = normalizeURL(page->url, url);
        if (normalized_url != NULL && isInternalURL(page->url, normalized_url)) {
            if (hashtable_insert(url_set, normalized_url, NULL)) {
                webpage_t *new_page = malloc(sizeof(webpage_t));

                new_page->url = normalized_url;
                new_page->depth = depth;
                new_page->html = NULL;
                new_page->length = 0;
                bag_insert(bag, new_page);
            } else {
                free(normalized_url);
            }
        }
        free(url);
    }

    pagescanner_reset();
}

// Function to crawl webpages
static void crawl_webpages(crawler_t *crawler, const char *pageDirectory, int maxDepth) {
    int uniqueID =1;
    while (!bag_is_empty(crawler->bag)) {
        webpage_t *current_page = bag_extract(crawler->bag);

        // Pause for one second between page fetches
        sleep(1);

        // Use pagefetcher to retrieve a web page
        current_page->html = download(current_page->url, &(current_page->length));

        // Use pagesaver to write the web page to the pageDirectory with a unique document ID
        int documentID = pagedir_save(current_page, pageDirectory, uniqueID);
        documentID*=2;
        uniqueID++;

        // Print progress log (optional)
        //fprintf(stdout, "Crawled: %s\n", current_page->url);

        if (current_page->depth < maxDepth) {
            // Explore the webpage to find the links it contains
            extract_and_add_links(current_page, crawler->bag, crawler->url_set);
        }

        // Free memory allocated for the current page
        free(current_page->url);
        free(current_page->html);
        free(current_page);
    }
}

// Function to initialize the crawler
void parseArgs(crawler_t *crawler, const char *seedURL, int maxDepth) {
    // Initialize the bag and hashtable
    crawler->bag = bag_new();
    crawler->url_set = hashtable_new(100); // Choose an appropriate number of slots

    if((maxDepth>10)||(maxDepth<0)) {
        fprintf(stderr,"Error: Range of Path must be from [0,10]\n"); exit(1);
    }

    // Add seedURL to the bag and hashtable
    hashtable_insert(crawler->url_set, seedURL, NULL);
    webpage_t *seed_page = malloc(sizeof(webpage_t));
    seed_page->url = strdup(seedURL);
    seed_page->depth = 0;
    seed_page->html = NULL;
    seed_page->length = 0;
    bag_insert(crawler->bag, seed_page);
}

// Function to run the crawler
void crawl(crawler_t *crawler, const char *pageDirectory, int maxDepth) {
    crawl_webpages(crawler, pageDirectory, maxDepth);

    // Clean up
    bag_delete(crawler->bag, free);
    hashtable_delete(crawler->url_set, NULL);
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command line arguments is provided
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <seedURL> <pageDirectory> <maxDepth>\n", argv[0]);
        return 1;
    }

    // Extract command line arguments
    const char *seedURL = argv[1];
    const char *pageDirectory = argv[2];
    int maxDepth = atoi(argv[3]);

    // Example usage of the crawler
    crawler_t myCrawler;
    parseArgs(&myCrawler, seedURL, maxDepth);
    crawl(&myCrawler, pageDirectory, maxDepth);

    return 0;
}
