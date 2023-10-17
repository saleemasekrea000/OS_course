#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void sigusr1_handler(int signum)
{
    char buffer[1024];
    int fd = open("text.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening text.txt");
        exit(1);
    }
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    close(fd);
}

void sigusr2_handler(int signum)
{
    printf("Process terminating...\n");
    exit(0);
}
int main()
{
    int pid_file = open("/var/run/agent.pid", O_CREAT | O_WRONLY, 0666);
    if (pid_file < 0)
    {
        perror("Error creating /var/run/agent.pid");
        exit(1);
    }
    dprintf(pid_file, "%d", getpid());
    printf("Agent PID: %d\n", getpid());
    close(pid_file);
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    char buffer[1024];
    int text_file = open("text.txt", O_RDONLY);
    if (text_file < 0)
    {
        perror("Error opening text.txt");
        exit(1);
    }
    ssize_t bytes_read;
    while ((bytes_read = read(text_file, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    close(text_file);
    while (1)
    {
        sleep(3600);
    }
    return 0;
}
