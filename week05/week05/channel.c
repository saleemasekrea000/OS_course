#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];
    pid_t child_pid;
    char buffer[BUFFER_SIZE];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if ((child_pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (child_pid == 0) {
        close(pipefd[1]);
        while (read(pipefd[0], buffer, BUFFER_SIZE) > 0) {
            printf("the Subscriber received: %s", buffer);
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    } else {
        close(pipefd[0]);
        printf("Publisher: Enter your message here : ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(pipefd[1], buffer, BUFFER_SIZE);
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
    }
}

