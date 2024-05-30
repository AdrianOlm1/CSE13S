#include <stdlib.h>
#include "bag.h"

// Define the structure for a bag node
typedef struct bagnode {
    void *item;
    struct bagnode *next;
} bagnode_t;

// Define the structure for a bag
struct bag {
    bagnode_t *head;
};

// Function to create a new bag
bag_t *bag_new(void) {
    bag_t *new_bag = malloc(sizeof(bag_t));
    if (new_bag == NULL) {
        // Handle memory allocation failure
        exit(EXIT_FAILURE);
    }
    new_bag->head = NULL;
    return new_bag;
}

// Function to insert an item into the bag
void bag_insert(bag_t *bag, void *item) {
    bagnode_t *new_node = malloc(sizeof(bagnode_t));
    if (new_node == NULL) {
        // Handle memory allocation failure
        exit(EXIT_FAILURE);
    }
    new_node->item = item;
    new_node->next = bag->head;
    bag->head = new_node;
}

// Function to extract an item from the bag
void *bag_extract(bag_t *bag) {
    if (bag->head == NULL) {
        // Bag is empty
        return NULL;
    }
    void *item = bag->head->item;
    bagnode_t *temp = bag->head;
    bag->head = bag->head->next;
    free(temp);
    return item;
}

// Function to check if the bag is empty
int bag_is_empty(bag_t *bag) {
    return (bag->head == NULL);
}

// Function to delete the bag
void bag_delete(bag_t *bag, void (*itemdelete)(void *item)) {
    while (bag->head != NULL) {
        bagnode_t *temp = bag->head;
        bag->head = bag->head->next;
        if (itemdelete != NULL) {
            itemdelete(temp->item);
        }
        free(temp);
    }
    free(bag);
}
