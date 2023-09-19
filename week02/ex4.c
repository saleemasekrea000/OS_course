#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count(char input[], char target)
{
    int count = 0;
    int len = strlen(input);

    target = tolower(target);

    for (int i = 0; i < len; i++)
    {
        if (tolower(input[i]) == target)
        {
            count++;
        }
    }

    return count;
}
void countAll(char input[])
{
    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        char currentChar = input[i];
        currentChar = tolower(currentChar);
        int charCount = count(input, currentChar);
        printf("%c:%d, ", currentChar, charCount);
    }
    printf("\n");
}
int main()
{
    char input[256];
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';
    char target;
    int charCount = count(input, target);
    printf("Count of all characters:\n");
    countAll(input);

    return 0;
}
