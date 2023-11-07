#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

struct PageTableEntry {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};
int numPages, numFrames;
void DisplayPageTable(struct PageTableEntry *pageTable) {
    printf("Page table\n");
    for (int i = 0; i < numPages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, pageTable[i].valid, pageTable[i].frame, pageTable[i].dirty, pageTable[i].referenced);
    }
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <numPages> <numFrames>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    numPages = atoi(argv[1]);
    numFrames = atoi(argv[2]);
    FILE *file = fopen("/tmp/pager.pid", "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    } else {
        pid_t processID = getpid();
        fprintf(file, "%d\n", processID);
        fclose(file);
    }
    char RAM[numFrames][8];
    char disk[numPages][8];

    size_t pageTableSize = sizeof(struct PageTableEntry) * numPages;
    int pageTableFile = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (pageTableFile == -1) {
        perror("Failed to open page table file");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(pageTableFile, pageTableSize) == -1) {
        perror("Failed to truncate page table file");
        close(pageTableFile);
        exit(EXIT_FAILURE);
    }
    struct PageTableEntry *pageTable = (struct PageTableEntry *)mmap(NULL, pageTableSize, PROT_READ | PROT_WRITE, MAP_SHARED, pageTableFile, 0);
    if (pageTable == MAP_FAILED) {
        perror("Failed to map page table");
        close(pageTableFile);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numPages; i++) {
        pageTable[i].valid = false;
        pageTable[i].frame = -1;
        pageTable[i].dirty = false;
        pageTable[i].referenced = 0;
    }
    printf("----------------------\n");
    DisplayPageTable(pageTable);
    printf("----------------------\n");
    printf("Initialized RAM\n");
    printf("RAM array\n");
    for (int i = 0; i < numFrames; i++) {
        strcpy(RAM[i], "empty");
        printf("Frame %d --->\n", i);
    }
    printf("----------------------\n");
    printf("Initialized disk\n");
    printf("Disk array\n");
    char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    for (int i = 0; i < numPages; i++) {
        char randomMessage[8];

        for (int j = 0; j < 7; j++) {
            randomMessage[j] = characters[rand() % (sizeof(characters) - 1)];
        }

        randomMessage[7] = '\0';
        strcpy(disk[i], randomMessage);
        printf("Page %d ----> %s\n", i, disk[i]);
    }
    int diskAccesses = 0;
    sigset_t signalSet;
    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGUSR1);
    sigprocmask(SIG_BLOCK, &signalSet, NULL);
    int signalReceived;
    while (sigwait(&signalSet, &signalReceived) == 0) {
        int foundNonZeroReference = -1;
        printf("----------------------\n");
        for (int i = 0; i < numPages; i++) {
            if (pageTable[i].referenced != 0) {
                printf("A disk access request from MMU Process (pid=%d)\n", pageTable[i].referenced);
                printf("Page %d is referenced\n", i);
                foundNonZeroReference = pageTable[i].referenced;
                bool isOk = false;

                for (int j = 0; j < numFrames; j++) {
                    if (strcmp(RAM[j], "empty") == 0) {
                        printf("We can allocate it to free frame %d\n", j);
                        printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, j);
                        pageTable[i].frame = j;
                        strcpy(RAM[j], disk[i]);
                        isOk = true;
                        pageTable[i].valid = 1;
                        pageTable[i].dirty = 0;
                        pageTable[i].referenced = 0;
                        diskAccesses++;
                        break;
                    }
                }
                if (!isOk) {
                    int randomFrame = rand() % numFrames;
                    printf("We do not have free frames in RAM\n");
                    printf("Chose a random victim page %d\n", randomFrame);
                    printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, randomFrame);
                    printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, randomFrame);
                    strcpy(RAM[randomFrame], disk[i]);

                    for (int j = 0; j < numPages; j++) {
                        if (pageTable[j].frame == randomFrame) {
                            pageTable[j].valid = 0;
                            pageTable[j].frame = -1;
                        }
                    }

                    pageTable[i].frame = randomFrame;
                    pageTable[i].valid = 1;
                    pageTable[i].dirty = 0;
                    pageTable[i].referenced = 0;
                    diskAccesses++;
                }
            }
        }
        if (foundNonZeroReference == -1) {
            printf("%d disk accesses in total\n", diskAccesses);
            printf("Pager is terminated\n");
            exit(0);
        } else {
            printf("RAM array\n");

            for (int i = 0; i < numFrames; i++) {
                printf("Frame %d ---> %s\n", i, RAM[i]);
            }

            printf("Disk accesses is %d so far\n", diskAccesses);
            printf("Resume MMU process\n");
            printf("%d\n", foundNonZeroReference);
            kill(foundNonZeroReference, SIGCONT);
        }
    }
}

