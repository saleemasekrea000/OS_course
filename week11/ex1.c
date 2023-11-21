#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *entry;

    // Open the current directory
    dir = opendir("/");

    // Check if the directory was opened successfully
    if (dir == NULL) {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    // Read and print directory entries
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);

    return EXIT_SUCCESS;
}
