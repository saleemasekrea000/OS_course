#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h> 
#define BUF_LEN (10 * (sizeof(struct inotify_event) + 257))
char initial_dir[1024];
void print_stat_info(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0)
    {
        printf("Stat info for %s:\n", path);
        printf("  Size: %ld\n", st.st_size);
        printf("  Inode: %ld\n", st.st_ino);
        printf("  Mode: %o\n", st.st_mode);
        printf("  UID: %d\n", st.st_uid);
        printf("  GID: %d\n", st.st_gid);
        printf("  Last access time: %ld\n", st.st_atime);
        printf("  Last modification time: %ld\n", st.st_mtime);
        printf("  Last status change time: %ld\n", st.st_ctime);
    }
    else
    {
        perror("stat");
    }
}
void handle_signal(int signo, const char *watched_path) {
    if (signo == SIGINT) {
        printf("\nPrinting stats info before termination:\n");
        printf("%s\n", initial_dir);
        DIR *dir;
        struct dirent *entry;
        char path[1024];
        strcpy(path, initial_dir);
        dir = opendir(path);
        if (dir == NULL) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {
                snprintf(path, sizeof(path), "%s/%s", initial_dir, entry->d_name);
                print_stat_info(path);
            }
        }
        closedir(dir);
        exit(0);
    }
}
void process_inotify_event(struct inotify_event *event, const char *watched_path) {
    char path[1024];
    strcpy(path, watched_path);
    strcat(path, "/");
    strcat(path, event->name);
    if (event->mask & IN_ACCESS) {
        printf("%s was accessed\n", path);
        print_stat_info(path);
    }
    if (event->mask & IN_CREATE) {
        printf("%s was created\n", path);
        print_stat_info(path);
    }
    if (event->mask & IN_DELETE) {
        printf("%s was deleted\n", path);
    }

    if (event->mask & IN_MODIFY) {
        printf("%s was modified\n", path);
        print_stat_info(path);
    }

    if (event->mask & IN_OPEN) {
        printf("%s was opened\n", path);
    }

    if (event->mask & IN_ATTRIB) {
        printf("Metadata of %s was changed\n", path);
        print_stat_info(path);
    }
    if (event->mask & (IN_MOVED_FROM | IN_MOVED_TO)) {
         if (strcmp(event->name, "") == 0) {
            printf("%s was moved (change the name of the directory)\n", watched_path);
            print_stat_info(watched_path);
        } else {
            printf("%s was moved (change the name of the file)\n", path);
            print_stat_info(path);
        }
    }
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    strcpy(initial_dir, argv[1]);
    signal(SIGINT, handle_signal);
    printf("Printing initial stat info:\n");
    DIR *dir;
    struct dirent *entry;
    char path[1024];
    strcpy(path, argv[1]);
    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            strcpy(path, argv[1]);
            strcat(path, "/");
            strcat(path, entry->d_name);
            print_stat_info(path);
        }
    }
    closedir(dir);
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }
    int watch_desc = inotify_add_watch(inotify_fd, argv[1], IN_ALL_EVENTS);
    if (watch_desc == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }
    char buf[BUF_LEN];
    ssize_t num_read;
    printf("Monitoring directory: %s\n", argv[1]);
    while (1) {
       char current_dir[1024];
        num_read = read(inotify_fd, buf, BUF_LEN);
        if (num_read == 0) {
            fprintf(stderr, "read() from inotify fd returned 0!");
            exit(EXIT_FAILURE);
        }
        struct inotify_event *event;
        for (char *ptr = buf; ptr < buf + num_read; ptr += sizeof(struct inotify_event) + event->len) {
            event = (struct inotify_event *)ptr;
            process_inotify_event(event, initial_dir);
        }
    }
    close(inotify_fd);

    return 0;
}

