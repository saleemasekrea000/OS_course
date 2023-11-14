#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>

#define PATH_MAX 4096

char path[PATH_MAX];

void find_all_hlinks(const char *source);
void unlink_all(const char *source);
void create_sym_link(const char *source, const char *link);
void sigint_handler(int signum);

void create_and_modify_files(const char *dir_path);
void move_and_modify_files(const char *dir_path);
void cleanup(const char *dir_path);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <watched_directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    snprintf(path, PATH_MAX, "%s", argv[1]);
    signal(SIGINT, sigint_handler);

    create_and_modify_files(argv[1]);
    move_and_modify_files(argv[1]);
    cleanup(argv[1]);

    return 0;
}

void find_all_hlinks(const char *source)
{
    struct stat file_stat;
    if (lstat(source, &file_stat) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }
    printf("Source File: %s\n", source);
    printf("Inode: %ld\n", (long)file_stat.st_ino);
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        struct stat entry_stat;
        if (lstat(entry->d_name, &entry_stat) == -1)
        {
            perror("lstat");
            exit(EXIT_FAILURE);
        }
        if (entry_stat.st_ino == file_stat.st_ino && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char resolved_path[PATH_MAX];
            realpath(entry->d_name, resolved_path);
            printf("Hard Link: Inode: %ld, Path: %s\n", (long)entry_stat.st_ino, resolved_path);
        }
    }

    closedir(dir);
}
void unlink_all(const char *source)
{
    struct stat file_stat;
    if (lstat(source, &file_stat) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    char last_link[PATH_MAX] = "";
    while ((entry = readdir(dir)) != NULL)
    {
        struct stat entry_stat;
        if (lstat(entry->d_name, &entry_stat) == -1)
        {
            perror("lstat");
            exit(EXIT_FAILURE);
        }
        if (entry_stat.st_ino == file_stat.st_ino && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if (last_link[0] != '\0')
            {
                if (unlink(entry->d_name) == -1)
                {
                    perror("unlink");
                    exit(EXIT_FAILURE);
                }
            }
            strcpy(last_link, entry->d_name);
        }
    }

    printf("Kept Hard Link: %s\n", last_link);

    closedir(dir);
}
void create_sym_link(const char *source, const char *link)
{
    if (symlink(source, link) == -1)
    {
        perror("symlink");
        exit(EXIT_FAILURE);
    }
}
void sigint_handler(int signum)

{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        struct stat entry_stat;
        if (lstat(entry->d_name, &entry_stat) == -1)
        {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        printf("Stat Info: %s - Inode: %ld\n", entry->d_name, (long)entry_stat.st_ino);
    }
    closedir(dir);
    exit(EXIT_SUCCESS);
}
void create_and_modify_files(const char *dir_path)
{
    char full[PATH_MAX];
    snprintf(full, PATH_MAX, "%s/myfile1.txt", dir_path);
    printf("%s\n", full);

    FILE *file1 = fopen(full, "w");
    if (file1 == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fprintf(file1, "Hello world.\n");
    fclose(file1);

    full[PATH_MAX];
    snprintf(full, PATH_MAX, "%s/myfile1.txt", dir_path);
    char second[PATH_MAX];
    snprintf(second, PATH_MAX, "%s/myfile11.txt", dir_path);
    link(full, second);
    full[PATH_MAX];
    snprintf(full, PATH_MAX, "%s/myfile1.txt", dir_path);
    second[PATH_MAX];
    snprintf(second, PATH_MAX, "%s/myfile12.txt", dir_path);
    link(full, second);
    find_all_hlinks(full);
}

void move_and_modify_files(const char *dir_path)
{
    char full[PATH_MAX];
    snprintf(full, PATH_MAX, "%s/myfile1.txt", dir_path);

    if (rename(full, "/tmp/myfile1.txt") == -1)
    {
        perror("rename");
        exit(EXIT_FAILURE);
    }

    char third[PATH_MAX];
    snprintf(third, PATH_MAX, "%s/myfile11.txt", dir_path);
    FILE *file11 = fopen(third, "a");
    if (file11 == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fprintf(file11, "Modification.");
    fclose(file11);
    third[PATH_MAX];
    snprintf(third, PATH_MAX, "%s/myfile13.txt", dir_path);
    create_sym_link("/tmp/myfile1.txt", third);
    FILE *tmp_file = fopen("/tmp/myfile1.txt", "a");
    if (tmp_file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fprintf(tmp_file, "Modification in /tmp/myfile1.txt.");
    fclose(tmp_file);
}

void cleanup(const char *dir_path)
{
    char third[PATH_MAX];
    snprintf(third, PATH_MAX, "%s/myfile11.txt", dir_path);
    unlink_all(third);
}
