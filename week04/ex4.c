#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 64
void execute_command(char **command_tokens, int is_background);
int main() {
    char user_input[MAX_INPUT_SIZE];
    char *user_tokens[MAX_TOKENS];
    while (1) {
        printf("MyShell> ");
        fflush(stdout);
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            break;
        }
        int token_count = 0;
        user_tokens[token_count] = strtok(user_input, " \t\n");
        while (user_tokens[token_count] != NULL) {
            token_count++;

            if (token_count >= MAX_TOKENS) {
                fprintf(stderr, "Too many arguments\n");
                break;
            }
            user_tokens[token_count] = strtok(NULL, " \t\n");
        }
        if (token_count == 0) {
            continue;
        }
        int is_background = 0;
        if (strcmp(user_tokens[token_count - 1], "&") == 0) {
            is_background = 1;
            user_tokens[token_count - 1] = NULL;
        }
        if (strcmp(user_tokens[0], "exit") == 0) {
            break;
        }
        execute_command(user_tokens, is_background);
    }
    return 0;
}
void execute_command(char **command_tokens, int is_background) {
    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        if (execvp(command_tokens[0], command_tokens) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        if (!is_background) {
            int status;
            waitpid(child_pid, &status, 0);
        }
    }
}

