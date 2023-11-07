#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
struct PageTableEntry {
    bool isValid;
    int frame;
    bool isDirty;
    int referenced;
};

struct TLBEntry {
    int page;
    int frame;
};

struct TLBEntry tlb[1000];
int tlbMissCount = 0;

int numPages;

void DisplayPageTable(struct PageTableEntry *pageTable) {
    printf("Page table\n");
    for (int i = 0; i < numPages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, pageTable[i].isValid, pageTable[i].frame, pageTable[i].isDirty, pageTable[i].referenced);
    }
}
void UpdateTLB(int page, int frame,tablesize) {
    for (int i = tablesize - 1; i > 0; i--) {
        tlb[i].page = tlb[i - 1].page;
        tlb[i].frame = tlb[i - 1].frame;
    }
    tlb[0].page = page;
    tlb[0].frame = frame;
}

void HandleSIGCONT(int signo) {
    // Handle the SIGCONT signal
    printf("Received SIGCONT signal. Resuming...\n");
}






int main(int argc, char *argv[]) {
    numPages = atoi(argv[1]);
    int pagerPID = atoi(argv[argc - 1]);
    int TLB_SIZE = (int)(numPages * 0.2); 
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].page = -1;  // Invalid page number
        tlb[i].frame = -1; // Invalid frame number
    }
    printf("Hello, pagerPID: %d\n", pagerPID);
    printf("Current process PID: %d\n", getpid());

    for (int i = 2; i < argc - 1; i++) {
        int pageTableFile = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

        if (pageTableFile == -1) {
            perror("Failed to open page table file");
            exit(EXIT_FAILURE);
        }

        int pageTableSize = sizeof(struct PageTableEntry) * numPages;
        struct PageTableEntry *pageTable = (struct PageTableEntry *)mmap(NULL, pageTableSize, PROT_READ | PROT_WRITE, MAP_SHARED, pageTableFile, 0);

        if (i == 2) {
            printf("----------------------\n");
            printf("Initialized page table\n");
            DisplayPageTable(pageTable);
        }

        char requestType = argv[i][0];
        int requestedPageNum = argv[i][1] - '0';
        int frameNumber = -1;
        // Check TLB for the requested page
        for (int j = 0; j < TLB_SIZE; j++) {
            if (tlb[j].page == requestedPageNum) {
                frameNumber = tlb[j].frame;
                break;
            }
        }
        if (frameNumber == -1) {
    // TLB miss
    tlbMissCount++;
    // Check the page table
    if (pageTable[requestedPageNum].isValid == 0) {
        // Handle page fault as before
        printf("It is not a valid page --> page fault\n");
        pageTable[requestedPageNum].referenced = getpid();
        kill(pagerPID, SIGUSR1);

        struct sigaction sa;
        sa.sa_handler = HandleSIGCONT;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGCONT, &sa, NULL);
        pause();
        // Update frameNumber after the page fault is resolved
        frameNumber = pageTable[requestedPageNum].frame;
    } else {
        frameNumber = pageTable[requestedPageNum].frame;
    }
    // Update TLB with the new entry
    UpdateTLB(requestedPageNum, frameNumber,TLB_SIZE);
}
double tlbMissRatio = (double)tlbMissCount / (argc - 2);
    printf("TLB Miss Ratio: %.2f\n", tlbMissRatio);

        if (requestType == 'W') {
            printf("----------------------\n");
            printf("Write Request for page %d\n", requestedPageNum);

            if (pageTable[requestedPageNum].isValid == 0) {
                printf("It is not a valid page --> page fault\n");
                pageTable[requestedPageNum].referenced = getpid();
                kill(pagerPID, SIGUSR1);

                struct sigaction sa;
                sa.sa_handler = HandleSIGCONT;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = 0;
                sigaction(SIGCONT, &sa, NULL);
                pause();

                pageTable[requestedPageNum].isDirty = 1;
            } else {
                printf("It is a valid page\n");
                pageTable[requestedPageNum].isDirty = 1;
            }
        } else if (requestType == 'R') {
            printf("----------------------\n");
            printf("Read Request for page %d\n", requestedPageNum);

            if (pageTable[requestedPageNum].isValid == 0) {
                printf("It is not a valid page --> page fault\n");
                pageTable[requestedPageNum].referenced = getpid();
                kill(pagerPID, SIGUSR1);

                struct sigaction sa;
                sa.sa_handler = HandleSIGCONT;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = 0;
                sigaction(SIGCONT, &sa, NULL);
                pause();
            } else {
                printf("It is a valid page\n");
            }
        }
        DisplayPageTable(pageTable);
    }
    kill(pagerPID, SIGUSR1);
}

