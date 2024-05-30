#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"

// Define the hashtable structure
struct hashtable {
    int num_slots;  // Number of bins in the hashtable
    set_t **bins;   // Array of sets to store (key, item) pairs
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
    new_hashtable->bins = malloc(num_slots * sizeof(set_t *));
    if (new_hashtable->bins == NULL) {
        free(new_hashtable);
        return NULL; // Allocation error
    }

    // Initialize each set in the array
    for (int i = 0; i < num_slots; i++) {
        new_hashtable->bins[i] = set_new();
        if (new_hashtable->bins[i] == NULL) {
            // Cleanup on error
            for (int j = 0; j < i; j++) {
                set_delete(new_hashtable->bins[j], NULL);
            }
            free(new_hashtable->bins);
            free(new_hashtable);
            return NULL; // Allocation error
        }
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
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false; // Invalid input
    }

    // Get the bin index using the hash function
    int bin_index = hash_function(key, ht->num_slots);

    // Insert the (key, item) pair into the corresponding set
    return set_insert(ht->bins[bin_index], key, item);
}

// Function to find an item in the hashtable based on the key
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL; // Invalid input
    }

    // Get the bin index using the hash function
    int bin_index = hash_function(key, ht->num_slots);

    // Find the item in the corresponding set
    return set_find(ht->bins[bin_index], key);
}

// Function to print the whole hashtable
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL) {
        return; // Invalid file pointer
    }

    // Print each set in the hashtable
    for (int i = 0; i < ht->num_slots; i++) {
        set_print(ht->bins[i], fp, itemprint);
    }
}

// Function to iterate over all items in the hashtable
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || itemfunc == NULL) {
        return; // Invalid input
    }

    // Call the given function on each item in each set
    for (int i = 0; i < ht->num_slots; i++) {
        set_iterate(ht->bins[i], arg, itemfunc);
    }
}

// Function to delete the whole hashtable
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return; // Nothing to delete
    }

    // Delete each set in the hashtable
    for (int i = 0; i < ht->num_slots; i++) {
        set_delete(ht->bins[i], itemdelete);
    }

    // Free the array of sets and the hashtable structure
    free(ht->bins);
    free(ht);
}
