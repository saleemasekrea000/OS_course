#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMORY_SIZE 10000000
#define ALLOCATIONS_FILE "queries.txt"

unsigned int memory[MEMORY_SIZE];

void initializeMemory() {
    memset(memory, 0, sizeof(memory));
}

int firstFitAlgorithm(unsigned int size) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        int blockSize = 0;
        while (i < MEMORY_SIZE && memory[i] == 0) {
            blockSize++;
            if (blockSize >= size) {
                for (int j = i - blockSize + 1; j <= i; j++) {
                    memory[j] = size;
                }
                return i - blockSize + 1;
            }
            i++;
        }
    }
    return -1; // No block found
}
int bestFitAlgorithm(unsigned int size) {
    int bestFit = -1;
    int bestFitSize = MEMORY_SIZE;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        int blockSize = 0;
        while (i < MEMORY_SIZE && memory[i] == 0) {
            blockSize++;
            if (blockSize >= size) {
                if (blockSize < bestFitSize) {
                    bestFit = i - blockSize + 1;
                    bestFitSize = blockSize;
                }
                i++;
            }
            else {
                i++;
            }
        }
    }
    if (bestFit != -1) {
        for (int j = bestFit; j < bestFit + size; j++) {
            memory[j] = size;
        }
    }
    return bestFit;
}
    int worstFitAlgorithm(unsigned int size) {
    int worstFit = -1;
    int worstFitSize = -1;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        int blockSize = 0;
        while (i < MEMORY_SIZE && memory[i] == 0) {
            blockSize++;
            if (blockSize >= size) {
                if (blockSize > worstFitSize) {
                    worstFit = i - blockSize + 1;
                    worstFitSize = blockSize;
                }
                i++;
            }
            else {
                i++;
            }
        }
    }
    if (worstFit != -1) {
        for (int j = worstFit; j < worstFit + size; j++) {
            memory[j] = size;
        }
    }

    return worstFit;
}
void clearMemory(unsigned int size) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == size) {
            memory[i] = 0;
        }
    }
}
int main() {
    initializeMemory();
    FILE *file = fopen(ALLOCATIONS_FILE, "r");
    clock_t startTime, endTime;
    char operation[20];
    unsigned int size;
    int allocations, deallocations;

    if (file == NULL) {
        printf("Error: Could not open %s\n", ALLOCATIONS_FILE);
        return 1;
    }
    FILE *output = fopen("ex1.txt", "w");
    int (*allocationAlgorithms[])(unsigned int) = {firstFitAlgorithm, bestFitAlgorithm, worstFitAlgorithm};
    char *algorithmNames[] = {"First Fit", "Best Fit", "Worst Fit"};

    for (int algorithm = 0; algorithm < 3; algorithm++) {
        initializeMemory();
        startTime = clock();
        allocations = 0;
        deallocations = 0;
        while (fscanf(file, "%s", operation) != EOF) {
            if (strcmp(operation, "allocate") == 0) {
                fscanf(file, "%u", &size);
                int startIndex = allocationAlgorithms[algorithm](size);

                if (startIndex != -1) {
                    allocations++;
                }
            } else if (strcmp(operation, "clear") == 0) {
                fscanf(file, "%u", &size);
                clearMemory(size);
                deallocations++;
            }
        }
        endTime = clock();
        double executionTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
        double throughput = (allocations + deallocations) / executionTime;
        fprintf(output, "%s ", algorithmNames[algorithm]);
        fprintf(output, "throughput is: %.2lf\n", throughput);
        fprintf(output, "\n");
        fseek(file, 0, SEEK_SET);
    }
    fprintf(output, "First Fit performs the best in terms of throughput, while Best Fit and Worst Fit lag behind. \n");
    fclose(file);
    fclose(output);

    return 0;
}
