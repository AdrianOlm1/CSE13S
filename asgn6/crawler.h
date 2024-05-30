#ifndef CRAWLER_H
#define CRAWLER_H

#include "bag.h"
#include "hashtable.h"

typedef struct {
    char *url;
    char *html;
    size_t length;
    int depth;
} webpage_t;

typedef struct {
    bag_t *bag;
    hashtable_t *url_set;
} crawler_t;

void crawler_init(crawler_t *crawler, const char *seedURL, const char *pageDirectory);
void crawler_run(crawler_t *crawler, const char *pageDirectory, int maxDepth);

#endif // CRAWLER_H
