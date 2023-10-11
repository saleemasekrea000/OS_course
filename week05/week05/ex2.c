#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
struct Thread {
    pthread_t id;
    int number;
    char message[256];
};
void *thread_function(void *arg) {
    struct Thread *thread_info = (struct Thread *)arg;
    printf("Thread %d is created\n", thread_info->number);
    printf("%s\n", thread_info->message);
    pthread_exit(NULL);
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    struct Thread threads[n];
    for (int i = 0; i < n; i++) {
        threads[i].number = i;
        snprintf(threads[i].message, sizeof(threads[i].message), "Hello from thread %d", i);
        pthread_create(&(threads[i].id), NULL, thread_function, &(threads[i]));
        pthread_join(threads[i].id, NULL);
    }
    return 0;
}
