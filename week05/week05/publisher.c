#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#define MESSAGE_SIZE 1024
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of subscribers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int num_subscribers = atoi(argv[1]);
    char fifo_path[256];
    char message[MESSAGE_SIZE];
    for (int i = 1; i <= num_subscribers; i++) {
        snprintf(fifo_path, sizeof(fifo_path), "/tmp/ex1/s%d", i);
        mkfifo(fifo_path, 0666);
    }
    printf("Publisher: Enter a message:");
    while (fgets(message, MESSAGE_SIZE, stdin)) {
        for (int i = 1; i <= num_subscribers; i++) {
            if (fork() == 0) {  
                snprintf(fifo_path, sizeof(fifo_path), "/tmp/ex1/s%d", i);
                int fifo_fd = open(fifo_path, O_WRONLY);
                write(fifo_fd, message, strlen(message) + 1);
                close(fifo_fd);
                exit(EXIT_SUCCESS);
            }
        }
    }
    exit(EXIT_SUCCESS);
}
