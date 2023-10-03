#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int dot_product(int start, int end, int* u, int* v) {
    int result = 0;
    for (int i = start; i < end; i++) {
        result += u[i] * v[i];
    }
    return result;
}
int main() {
    int n;
    printf("Enter the number of processes (n): ");
    scanf("%d", &n);
    srand(time(NULL));
    int u[120];
    int v[120];
    for (int i = 0; i < 120; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }
    FILE* file = fopen("temp.txt", "a");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }
    pid_t pid;
    for (int i = 0; i < n; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            return 1;
        } else if (pid == 0) {
            int start = i * (120 / n);
            int end = (i + 1) * (120 / n);
            int result = dot_product(start, end, u, v);
            fprintf(file, "%d\n", result);
            /*printf("%d\n", result);*/
            fclose(file); 
            exit(0);
        }
    }
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    file = fopen("temp.txt", "r");
    int total_result = 0;
    int result;
    while (fscanf(file, "%d", &result) == 1) {
        total_result += result;
    }
    fclose(file);
    printf("The dot product of u and v is: %d\n", total_result);
    return 0;
}