#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILENAME_LENGTH 63
#define MAX_PATH_LENGTH 2048
#define MAX_FILE_DATA_SIZE 1024
#define MAX_FILES_IN_DIRECTORY 100
#define MAX_SUBDIRECTORIES_IN_DIRECTORY 100

struct File {
    unsigned int id;
    char name[MAX_FILENAME_LENGTH + 1]; // File name (+1 for null terminator)
    unsigned short size; // with the terminator
    char data[MAX_FILE_DATA_SIZE];
    struct Directory* directory;
};
struct Directory {
    char name[MAX_FILENAME_LENGTH + 1];
    struct File* files[MAX_FILES_IN_DIRECTORY];
    struct Directory* directories[MAX_SUBDIRECTORIES_IN_DIRECTORY];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH_LENGTH];
};
void overwriteToFile(struct File* file, const char* str) {
    if (file != NULL && str != NULL) {
        strncpy(file->data, str, MAX_FILE_DATA_SIZE);
        file->size = (unsigned short)strlen(file->data) + 1;
    }
}
void appendToFile(struct File* file, const char* str) {
    if (file != NULL && str != NULL) {
        int remainingSpace = MAX_FILE_DATA_SIZE - file->size;
        if (remainingSpace > 1) {
            strncat(file->data, str, remainingSpace - 1);
            file->size = (unsigned short)strlen(file->data) + 1;
        }
    }
}
void addFileToDirectory(struct File* file, struct Directory* dir) {
    if (file != NULL && dir != NULL && dir->nf < MAX_FILES_IN_DIRECTORY) {
        file->directory = dir;
        dir->files[dir->nf++] = file;
    }
}
void printFilePath(struct File* file) {
    if (file != NULL) {
        printf("%s/%s\n", file->directory->path, file->name);
    }
}

int main() {
    struct Directory root;
    strcpy(root.name, "/");
    root.nf = 0;
    root.nd = 0;
    strcpy(root.path, "/");

    struct Directory home;
    strcpy(home.name, "home");
    home.nf = 0;
    home.nd = 0;
    snprintf(home.path, MAX_PATH_LENGTH, "/%s", home.name);
    root.directories[root.nd++] = &home;

    struct Directory bin;
    strcpy(bin.name, "bin");
    bin.nf = 0;
    bin.nd = 0;
    snprintf(bin.path, MAX_PATH_LENGTH, "/%s", bin.name);
    root.directories[root.nd++] = &bin;

    struct File bash;
    bash.id = 1;
    strcpy(bash.name, "bash");
    strcpy(bash.data, "Bourne Again Shell!!");
    bash.size = (unsigned short)strlen(bash.data) + 1;
    addFileToDirectory(&bash, &bin);

    struct File ex3_1;
    ex3_1.id = 2;
    strcpy(ex3_1.name, "ex3_1.c");
    strcpy(ex3_1.data, "int printf(const char * format, ...);");
    ex3_1.size = (unsigned short)strlen(ex3_1.data) + 1;
    addFileToDirectory(&ex3_1, &home);

    struct File ex3_2;
    ex3_2.id = 3;
    strcpy(ex3_2.name, "ex3_2.c");
    strcpy(ex3_2.data, "//This is a comment in C language");
    ex3_2.size = (unsigned short)strlen(ex3_2.data) + 1;
    addFileToDirectory(&ex3_2, &home);
    appendToFile(&ex3_1, "int main(){printf(\"Hello World!\");}");

    printf("Paths of all files:\n");
    printFilePath(&bash);
    printFilePath(&ex3_1);
    printFilePath(&ex3_2);

    return 0;
}
