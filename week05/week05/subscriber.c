#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define MESSAGE_SIZE 1024
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <subscriber id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int subscriber_id = atoi(argv[1]);
    char fifo_path[256];
    snprintf(fifo_path, sizeof(fifo_path), "/tmp/ex1/s%d", subscriber_id);
    char message[MESSAGE_SIZE];
    int fifo_fd = open(fifo_path, O_RDONLY);
    while (1) {
        ssize_t bytes_read = read(fifo_fd, message, MESSAGE_SIZE);
        if (bytes_read > 0) {
            printf("Subscriber %d received: %s", subscriber_id, message);
        } else if (bytes_read < 0) {
            break;
        }
    }
    close(fifo_fd);
    exit(EXIT_SUCCESS);
}
