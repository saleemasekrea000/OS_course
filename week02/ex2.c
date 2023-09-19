#include <stdio.h>
#include <string.h>
#define MAX_SIZE 256
int main() {
    char input[MAX_SIZE];
    char character;
    int length = 0;
    printf("Enter a string character-by-character,maximum charactersis 256:\n");
    while (1) {
        character = getchar();
        if (character == '.' || character == '\n') {
            input[length] = '\0';
            break;
        }
        input[length++] = character;
        if (length >= MAX_SIZE - 1) {
            printf("Maximum size reached\n");
            input[length] = '\0';
            break;
        }
    }
    printf("\"");
    for (int i = strlen(input)-1; i >= 0; i--) {
        putchar(input[i]);
    }
    printf("\"\n");

    return 0;
}
