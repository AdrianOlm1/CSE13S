#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

// Define a structure for each (key, item) pair
typedef struct entry {
    char *key;
    void *item;
    struct entry *next;
} entry_t;

// Define the hashtable structure
struct hashtable {
    int num_slots;    // Number of bins in the hashtable
    entry_t **bins;   // Array of linked lists to store (key, item) pairs
};


// Function to create a new (empty) hashtable
hashtable_t *hashtable_new(const int num_slots) {
    if (num_slots <= 0) {
        return NULL; // Invalid number of slots
    }

    // Allocate memory for the hashtable
    hashtable_t *new_hashtable = malloc(sizeof(hashtable_t));
    if (new_hashtable == NULL) {
        return NULL; // Allocation error
    }

    // Set the number of bins
    new_hashtable->num_slots = num_slots;

    // Allocate memory for the array of sets
    new_hashtable->bins = malloc(num_slots * sizeof(hashtable_t *));
    if (new_hashtable->bins == NULL) {
        free(new_hashtable);
        return NULL; // Allocation error
    }

    // Initialize each bin in the array
    for (int i = 0; i < num_slots; i++) {
        new_hashtable->bins[i] = NULL;
    }

    return new_hashtable;
}

// Function to hash a key and return the corresponding bin index
static int hash_function(const char *key, int num_slots) {
    // Simple hash function: sum the ASCII values of characters in the key
    int hash_value = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash_value += key[i];
    }
    return hash_value % num_slots;
}

// Function to insert an item into the hashtable
int hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL){// || item == NULL) {
        return false; // Invalid input
    }

    // Get the bin index using the hash function
    int bin_index = hash_function(key, ht->num_slots);

    // Check if the key already exists in the hashtable
    entry_t *current = ht->bins[bin_index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return false; // Key already exists, return false
        }
        current = current->next;
    }

    // Key does not exist, create a new entry and insert it at the beginning of the list
    entry_t *new_entry = malloc(sizeof(entry_t));
    if (new_entry == NULL) {
        return false; // Allocation error
    }
    new_entry->key = strdup(key);
    if (new_entry->key == NULL) {
        free(new_entry);
        return false; // Allocation error
    }
    new_entry->item = item;
    new_entry->next = ht->bins[bin_index];
    ht->bins[bin_index] = new_entry;

    return true; // Item inserted successfully
}


// Function to find an item in the hashtable based on the key
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL; // Invalid input
    }

    // Get the bin index using the hash function
    int bin_index = hash_function(key, ht->num_slots);

    // Find the item in the corresponding linked list
    entry_t *current = ht->bins[bin_index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->item; // Key found
        }
        current = current->next;
    }

    return NULL; // Key not found
}

// Function to print the whole hashtable
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL || ht == NULL) {
        return; // Invalid file pointer or hashtable
    }

    // Print each (key, item) pair in the hashtable
    for (int i = 0; i < ht->num_slots; i++) {
        entry_t *current = ht->bins[i];
        while (current != NULL) {
            itemprint(fp, current->key, current->item);
            current = current->next;
        }
    }
}

// Function to iterate over all items in the hashtable
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || itemfunc == NULL) {
        return; // Invalid input
    }

    // Call the given function on each (key, item) pair in the hashtable
    for (int i = 0; i < ht->num_slots; i++) {
        entry_t *current = ht->bins[i];
        while (current != NULL) {
            itemfunc(arg, current->key, current->item);
            current = current->next;
        }
    }
}

// Function to delete the whole hashtable
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return; // Nothing to delete
    }

    // Delete each linked list in the hashtable
    for (int i = 0; i < ht->num_slots; i++) {
        entry_t *current = ht->bins[i];
        while (current != NULL) {
            entry_t *next = current->next;
            free(current->key);
            if (itemdelete != NULL) {
                itemdelete(current->item);  // Call itemdelete function
            }
            free(current);
            current = next;
        }
    }

    // Free the array of linked lists and the hashtable structure
    free(ht->bins);
    free(ht);
}
