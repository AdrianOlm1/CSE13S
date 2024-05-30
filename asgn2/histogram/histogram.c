#include <stdio.h>
#include <stdlib.h>

#define NUM_BINS 16

int main() {
    int range = 16;
    int bin_size = 1;
    int* bins = NULL; // Initialize the bins pointer

    // Allocate memory for bins
    bins = (int*)malloc(NUM_BINS * sizeof(int));
    if (bins == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize the bins
    for (int i = 0; i < NUM_BINS; i++) {
        bins[i] = 0;
    }

    printf("%d bins of size %d for range [0,%d)\n", NUM_BINS, bin_size, range);

    while (1) {
        int value;
        // Set value as inputed number and exits if end of file
        if (scanf("%d", &value) == EOF) {
            break;
        }
        
        // Checks to make sure the number is 0 or greater
        if(value>-1) {
            // Checks to see if the inputted value is greater than or equal to the range and repeats until the value is in the range
            while (value >= range) {
                // Increases range and bin size to allow for the value
                range *= 2;
                bin_size *= 2;
                printf("%d bins of size %d for range [0,%d)\n", NUM_BINS, bin_size, range);
                
                int* newBins = (int*)malloc(NUM_BINS * sizeof(int));

                // Initialize newBins
                for (int i = 0; i < NUM_BINS; i++) {
                    newBins[i] = 0;
                }

                // Transfer data from bins to newBins
                for (int i = 0; i < NUM_BINS; i++) {
                    // Distribute values to new bins
                    int new_bin_index = i / 2;
                    newBins[new_bin_index] += bins[i];
                }

                free(bins); // Free the old bins

                bins = newBins; // Update bins to the newBins
            }

            // Adds the inputed value to appropriate bin index
            int bin_index = (value / bin_size);
            bins[bin_index]++;
        }
    }

    // The printing of the counted bins at the end
    for (int i = 0; i < NUM_BINS; i++) {
        // Calculates the bin range for each bin and prints that
        int bin_start = i * bin_size;
        int bin_end = bin_start + bin_size - 1;
        printf("[%3d:%3d] ", bin_start, bin_end);

        //Prints a "*" for every occurence inputed in those bin ranges
        for (int j = 0; j < bins[i]; j++) {
            printf("*");
        }
        printf("\n");
    }

    free(bins); // Free the allocated memory

    return 0;
}
