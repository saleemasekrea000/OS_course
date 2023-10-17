#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void send_signal(int pid, int signum)
{
    if (kill(pid, signum) == -1)
    {
        perror("Error sending signal");
        exit(1);
    }
}
int main()
{
    FILE *pid_file = fopen("/var/run/agent.pid", "r");
    if (pid_file == NULL)
    {
        printf("Error: No agent found.\n");
        exit(1);
    }
    pid_t agent_pid;
    fscanf(pid_file, "%d", &agent_pid);
    fclose(pid_file);
    printf("Agent found.\n");
    printf("Agent PID: %d\n", agent_pid);
    while (1)
    {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"}: ");
        char choice[10];
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = '\0'; 
        if (strcmp(choice, "read") == 0)
        {
            send_signal(agent_pid, SIGUSR1);
        }
        else if (strcmp(choice, "exit") == 0)
        {
            send_signal(agent_pid, SIGUSR2);
            exit(0);
        }
        else if (strcmp(choice, "stop") == 0)
        {
            send_signal(agent_pid, SIGSTOP);
        }
        else if (strcmp(choice, "continue") == 0)
        {
            send_signal(agent_pid, SIGCONT);
        }
        else
        {
            printf("Invalid command. Please choose from {\"read\", \"exit\", \"stop\", \"continue\"}.\n");
        }
    }
    return 0;
}
