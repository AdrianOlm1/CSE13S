#ifndef LRU_H_
#define LRU_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct lru lru_t;

/* Create a new (empty) LRU; return NULL if error. */
lru_t *lru_new(const int capacity);

/* Insert item, identified by key (string), into the given LRU.
 * The key string is copied for use by the LRU.
 * Return false if key exists in LRU, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool lru_insert(lru_t *lru, const char *key, void *item);

/* Return the item associated with the given key;
 * return NULL if LRU is NULL, key is NULL, key is not found.
 */
void *lru_find(lru_t *lru, const char *key);

/* Print the whole LRU; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL LRU.
 * Print a table with no items if NULL itemprint.
 */
void lru_print(lru_t *lru, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item));

/* Iterate over all items in the LRU; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If LRU==NULL or itemfunc==NULL, do nothing.
 */
void lru_iterate(lru_t *lru, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item));

/* Delete the whole LRU; ignore NULL LRU.
 * Provide a function that will delete each item (may be NULL).
 */
void lru_delete(lru_t *lru, void (*itemdelete)(void *item));

#endif // LRU_H_
