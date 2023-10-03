#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
int main() {
    pid_t pid1, pid2;
    clock_t t1,t2,p;
    p=clock();
    pid1 = fork();
    t1 = clock();
    if (pid1 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid1 == 0) {
        printf("Child Process 1 - PID: %d, PPID: %d\n", getpid(), getppid());
        t1 = clock()-t1;
        printf("Execution time for Child Process 1: %.2f ms\n", (double)(t1) * 1000.0 / CLOCKS_PER_SEC);
        exit(0);
    }
    wait(NULL);
    pid2 = fork();
    t2 = clock();
    if (pid2 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid2 == 0) {
        printf("Child Process 2 - PID: %d, PPID: %d\n", getpid(), getppid());
        t2 = clock()-t2;
        printf("Execution time for Child Process 2: %.2f ms\n", (double)(t2) * 1000.0 / CLOCKS_PER_SEC);
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    p = clock()-p;
    printf("Parent Process - PID: %d\n", getpid());
    printf("Execution time for Parent Process: %.2f ms\n", (double)(p) * 1000.0 / CLOCKS_PER_SEC);
    return 0;
}
