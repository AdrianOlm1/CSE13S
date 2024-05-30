#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pagedir.h"

// Function to initialize the page directory
bool pagedir_init(const char *pageDirectory) {
    char *filepath = malloc(strlen(pageDirectory) + 12); // 12 = length of ".crawler" + 1
    printf("called\n");
    if (filepath == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    sprintf(filepath, "%s/.crawler", pageDirectory);
    FILE *file = fopen(filepath, "w");
    free(filepath);

    if (file == NULL) {
        fprintf(stderr, "Unable to create .crawler file\n");
        return false;
    }

    fclose(file);
    return true;
}

// Function to create a directory if it doesn't exist
static void create_directory(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700); // Create the directory with read, write, and execute permissions for owner only
        printf("No File Found under '%s', Creating File...\n", path);
    }
}

// Function to save a webpage to the page directory
int pagedir_save(const webpage_t *page, const char *pageDirectory, int uniqueID) {

    create_directory(pageDirectory);  // Create directory if it doesn't exist

    // Create the .crawler file in the specified directory
    char crawler_file[255]; // Adjust the size as needed

    sprintf(crawler_file, "%s/.crawler", pageDirectory);

    FILE *crawler_file_ptr = fopen(crawler_file, "w");
    if (crawler_file_ptr == NULL) {
        fprintf(stderr, "Unable to create .crawler file in %s\n", pageDirectory);
        exit(EXIT_FAILURE);
    }

    fclose(crawler_file_ptr);    

    char filename[255]; // Adjust the size as needed
    sprintf(filename, "%s/%d", pageDirectory, uniqueID);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to create or write to a file: %s\n", filename);
        return 1;
    }

    // Write URL, depth, and HTML content to the file
    fprintf(file, "%s\n%d\n%s", page->url, page->depth, page->html);

    fclose(file);
    return 0;; // Return document ID
}
