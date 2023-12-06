#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int MAX_RESOURCES =3;
int  MAX_THREADS =4;
int d=5;
int thread_status[100][100];
int graph[100][100];
pthread_mutex_t mutexes[100];
pthread_t threads[100];
int is_deadlock =0;
FILE *file;
struct Thread
{
    int thread_id;
    int resource_id;
};
void *thread_function (void *arg);
void print_locked_mutexes (int thread_id);
void read_requests ();
void clean_up ();
void destroy_mutexes();
int detect_deadlock (int current_node, int visited[], int parent);
void init_mutexes ();
int deadlock();
int main (int argc, char *argv[])
{
    file = fopen("ex1.txt", "w");
    if (file == NULL) {
        perror("Error opening ex1.txt");
        exit(EXIT_FAILURE);
    }
    
    if (argc != 3)
    {
        fprintf (stderr, "Usage: %s <m> <n>\n", argv[0]);
        exit (1);
    }
    int m = atoi (argv[1]);
    int n = atoi (argv[2]);
    MAX_RESOURCES=m;
    MAX_THREADS=m;
    init_mutexes ();
    read_requests ();
    clean_up ();
    destroy_mutexes();
    int b=deadlock();
    if(!b){
    printf("No deadlock");
    fprintf(file,"No deadlock");
    }
    return 0;
}

void clean_up ()
{
    for (int i = 0; i < MAX_THREADS; i++)
    {
        pthread_join (threads[i], NULL);
    }

}

void destroy_mutexes(){
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        pthread_mutex_destroy (&mutexes[i]);
    }
}
void read_requests ()
{
    freopen ("input.txt", "r", stdin);
    int thread_id, resource_id;

    // Read resource requests from the input file and create threads
    while (fscanf (stdin, "%d %d", &thread_id, &resource_id) == 2)
    {
        // Allocate memory for parameters to pass to thread_function
        struct Thread *params =
            (struct Thread *) malloc (sizeof (struct Thread));
        params->thread_id = thread_id;
        params->resource_id = resource_id;
        pthread_create (&threads[thread_id], NULL, thread_function,
                        (void *) params);
        // Initialize the graph based on resource requests
        graph[thread_id][resource_id] = 1;
    }

}

void print_locked_mutexes (int thread_id)
{
    printf ("Thread %d is holding mutexes: ", thread_id);
    fprintf(file,"Thread %d is holding mutexes: ", thread_id);
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        if (thread_status[thread_id][i] == 1)
        {
            printf ("%d ", i);
            fprintf(file,"%d ", i);
        }
    }
    printf ("\n");
    fprintf(file,"\n");
}

int detect_deadlock (int current_node, int visited[], int parent)
{
    visited[current_node] = 1;

    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        if (graph[current_node][i] == 1)
        {
            if (!visited[i])
            {
                if (detect_deadlock (i, visited, current_node))
                {
                    return 1;// Cycle detected
                }
            }
            else if (i != parent)
            {
                return 1;// Back edge detected
            }
        }
    }

    return 0;
}
int deadlock(){
	if(is_deadlock)
	   return 1;
	else
	  return 0;
}
void init_mutexes ()
{
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        pthread_mutex_init (&mutexes[i], NULL);
    }
}
void *thread_function (void *arg)
{
    struct Thread *params = (struct Thread *) arg;
    int thread_id = params->thread_id;
    int resource_id = params->resource_id;
    // Free the memory allocated in the main function
    free (params);		
    printf ("Thread %d is created.\n", thread_id);
    fprintf(file, "Thread %d is created.\n", thread_id);
    //  Print the ids of mutexes currently locked by the thread i
    print_locked_mutexes (thread_id);
    printf ("Thread %d tries to lock mutex %d.\n", thread_id, resource_id);
    fprintf("Thread %d tries to lock mutex %d.\n", thread_id, resource_id);
    // Check if it cannot lock and there is a deadlock
    if (pthread_mutex_trylock (&mutexes[resource_id]) != 0)
    {
        //printf("Thread %d is detected by sssss  .\n", thread_id);
        // printf("Deadlock is detected.\n");
        int visited[MAX_THREADS] ;
        memset(visited, 0, sizeof(visited));
        for (int i = 0; i < MAX_THREADS; i++)
        {
            if (!visited[i])
            {
                if (detect_deadlock (i, visited, -1))
                {
                    printf ("Deadlock is detected.\n");
                    fprintf (file,"Deadlock is detected.\n");
                    is_deadlock=1;
                    // Print the status of all threads (which mutexes are locked and requested by which threads)
                    for (int i = 0; i < MAX_THREADS; i++)
                    {
                        print_locked_mutexes (i);
                    }
                    destroy_mutexes();
                    exit (1);
                }
            }
        }
    }
    else
    {
        thread_status[thread_id][resource_id] = 1;
        printf ("Thread %d  lock the mutex %d.\n", thread_id, resource_id);
        fprintf (file,"Thread %d  lock the mutex %d.\n", thread_id, resource_id);

    }
    //Sleep for a random number of seconds (0 < x < 5)
    srand (time (NULL));
    int sleep_time = rand () % 5;
    sleep (sleep_time);
    pthread_mutex_unlock (&mutexes[resource_id]);
    printf ("Thread %d unlocked mutex %d.\n", thread_id, resource_id);
    fprintf (file,"Thread %d unlocked mutex %d.\n", thread_id, resource_id);
    // Terminate the thread if all acquired mutexes are unlocked
    thread_status[thread_id][resource_id] = 0;
    pthread_exit (NULL);
}
