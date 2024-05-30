#define _GNU_SOURCE 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

// Define a structure for each node in the linked list
typedef struct node {
    char *key;
    void *item;
    struct node *next;
} node_t;

// Define the set structure
struct set {
    node_t *head;
};

// Function to create a new (empty) set
set_t *set_new(void) {
    set_t *new_set = malloc(sizeof(set_t));
    if (new_set == NULL) {
        return NULL; // Allocation error
    }
    new_set->head = NULL;
    return new_set;
}

// Function to insert an item into the set
bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL || item == NULL) {
        return false; // Invalid input
    }

    // Check if the key already exists in the set
    node_t *current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return false; // Key already exists
        }
        current = current->next;
    }

    // Key does not exist, create a new node and insert it at the beginning of the list
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return false; // Allocation error
    }
    new_node->key = strdup(key);
    if (new_node->key == NULL) {
        free(new_node);
        return false; // Allocation error
    }
    new_node->item = item;
    new_node->next = set->head;
    set->head = new_node;

    return true; // Item inserted successfully
}

// Function to find an item in the set based on the key
void *set_find(set_t *set, const char *key) {
    if (set == NULL || key == NULL) {
        return NULL; // Invalid input
    }

    // Search for the key in the set
    node_t *current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->item; // Key found
        }
        current = current->next;
    }

    return NULL; // Key not found
}

// Function to print the whole set
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL) {
        return; // Invalid file pointer
    }

    // Print each item in the set
    node_t *current = set->head;
    while (current != NULL) {
        itemprint(fp, current->key, current->item);
        current = current->next;
    }
}

// Function to iterate over all items in the set
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (set == NULL || itemfunc == NULL) {
        return; // Invalid input
    }

    // Call the given function on each item in the set
    node_t *current = set->head;
    while (current != NULL) {
        itemfunc(arg, current->key, current->item);
        current = current->next;
    }
}

// Function to delete the whole set
void set_delete(set_t *set, void (*itemdelete)(void *item)) {
    if (set == NULL) {
        return; // Nothing to delete
    }

    // Delete each node in the set
    node_t *current = set->head;
    while (current != NULL) {
        node_t *next = current->next;
        free(current->key);
        if (itemdelete != NULL) {
            itemdelete(current->item);
        }
        free(current);
        current = next;
    }

    // Free the set structure
    free(set);
}
