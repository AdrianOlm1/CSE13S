#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char *s);

#include "lru.h"
#include "set.h"

// Define a structure for each node in the LRU
typedef struct lru_node {
    char *key;
    void *item;
    struct lru_node *next;
} lru_node_t;

// Define the LRU structure
struct lru {
    set_t *set;  // Hashtable for fast key lookup
    lru_node_t *head;  // Linked list to maintain the order of access
    int capacity;  // Maximum capacity of the LRU
    int size;  // Current size of the LRU
};

// Function to create a new (empty) LRU
lru_t *lru_new(const int capacity) {
    if (capacity <= 0) {
        return NULL;  // Invalid capacity
    }

    lru_t *new_lru = malloc(sizeof(lru_t));
    if (new_lru == NULL) {
        return NULL;  // Allocation error
    }

    new_lru->set = set_new();
    if (new_lru->set == NULL) {
        free(new_lru);
        return NULL;  // Allocation error
    }

    new_lru->head = NULL;
    new_lru->capacity = capacity;
    new_lru->size = 0;

    return new_lru;
}

// Function to insert an item into the LRU
bool lru_insert(lru_t *lru, const char *key, void *item) {

    if (lru_find(lru, key)) {
        // Key already exists, handle it as needed
        return false;
    }

    if (lru == NULL || key == NULL || item == NULL) {
        return false;  // Invalid input
    }

    // Check if the key already exists in the LRU
    if (set_find(lru->set, key) != NULL) {
        // Key already exists, update the item and move the node to the front
        lru_node_t *current = lru->head;
        lru_node_t *prev = NULL;
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                current->item = item;

                // Move the current node to the front
                if (prev != NULL) {
                    prev->next = current->next;
                    current->next = lru->head;
                    lru->head = current;
                }

                return true;  // Item updated successfully
            }
            prev = current;
            current = current->next;
        }
    }

    // Key does not exist, create a new node and insert it at the front
    lru_node_t *new_node = malloc(sizeof(lru_node_t));
    if (new_node == NULL) {
        return false;  // Allocation error
    }
    new_node->key = strdup(key);
    if (new_node->key == NULL) {
        free(new_node);
        return false;  // Allocation error
    }
    new_node->item = item;
    new_node->next = lru->head;
    lru->head = new_node;

    // Insert the key into the set for fast lookup
    if (!set_insert(lru->set, key, item)) {
        // Rollback the insertion in the linked list if set insertion fails
        lru_node_t *temp = lru->head;
        lru->head = lru->head->next;
        free(temp->key);
        free(temp);
        return false;  // Set insertion error
    }

    // Check if the LRU is full; if so, remove the least recently used item
    if (lru->size == lru->capacity) {
        lru_node_t *last = lru->head;
        lru_node_t *prev = NULL;
        while (last->next != NULL) {
            prev = last;
            last = last->next;
        }

        // Remove the least recently used item from the set
        set_delete(lru->set, NULL);

        // Remove the least recently used item from the linked list
        if (prev != NULL) {
            prev->next = NULL;
        } else {
            lru->head = NULL;
        }

        // Free the memory of the removed node
        free(last->key);
        free(last);
    } else {
        // Increment the size of the LRU
        lru->size++;
    }

    return true;  // Item inserted successfully
}

// Function to find an item in the LRU based on the key
void *lru_find(lru_t *lru, const char *key) {
    if (lru == NULL || key == NULL) {
        return NULL;  // Invalid input
    }

    // Search for the key in the set
    return set_find(lru->set, key);
}

// Function to print the whole LRU
void lru_print(lru_t *lru, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL) {
        return;  // Invalid file pointer
    }

    // Print each item in the LRU
    lru_node_t *current = lru->head;
    while (current != NULL) {
        itemprint(fp, current->key, current->item);
        current = current->next;
    }
}

// Function to iterate over all items in the LRU
void lru_iterate(lru_t *lru, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (lru == NULL || itemfunc == NULL) {
        return;  // Invalid input
    }

    // Call the given function on each item in the LRU
    lru_node_t *current = lru->head;
    while (current != NULL) {
        itemfunc(arg, current->key, current->item);
        current = current->next;
    }
}

// Function to delete the whole LRU
void lru_delete(lru_t *lru, void (*itemdelete)(void *item)) {
    if (lru == NULL) {
        return;  // Nothing to delete
    }

    // Delete each node in the LRU
    lru_node_t *current = lru->head;
    while (current != NULL) {
        lru_node_t *next = current->next;
        free(current->key);
        if (itemdelete != NULL) {
            itemdelete(current->item);
        }
        free(current);
        current = next;
    }

    // Free the set structure
    set_delete(lru->set, NULL);

    // Free the LRU structure
    free(lru);
}
