#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DISK_SIZE 128 * 1024
#define SUPER_BLOCK_SIZE 1024
#define INODE_SIZE 56
#define MAX_FILES 16
#define MAX_FILE_NAME 16
#define BLOCK_SIZE 1024
#define MAX_FILE_SIZE 8


struct superblock {
    char free_blocks[BLOCK_SIZE];
    struct inode inodes[MAX_FILES];
};

struct inode {
    char name[MAX_FILE_NAME];
    int size;
    int blockPointers[MAX_FILE_SIZE];
    int used;
};


char disk[DISK_SIZE];

// Function prototypes
void initialize_superblock(struct superblock *sb);
void print_file_system(struct superblock *sb);
int create_file(struct superblock *sb, const char *filename, int size);
void delete_file(struct superblock *sb, const char *filename);
void read_block(struct superblock *sb, const char *filename, int blockNum, char *buf);
void write_block(struct superblock *sb, const char *filename, int blockNum, const char *buf);
void list_files(struct superblock *sb);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    struct superblock *sb = (struct superblock *)disk;


    initialize_superblock(sb);

    char operation;
    char filename[MAX_FILE_NAME];
    int size, blockNum;
    char buf[BLOCK_SIZE];


    while (fscanf(inputFile, "%c", &operation) != EOF) {
        switch (operation) {
            case 'C':
                fscanf(inputFile, "%s %d", filename, &size);
                create_file(sb, filename, size);
                break;

            case 'D':
                fscanf(inputFile, "%s", filename);
                delete_file(sb, filename);
                break;

            case 'L':
                list_files(sb);
                break;

            case 'R':
                fscanf(inputFile, "%s %d", filename, &blockNum);
                read_block(sb, filename, blockNum, buf);
                break;

            case 'W':
                fscanf(inputFile, "%s %d", filename, &blockNum);

                sprintf(buf, "Dummy data for block %d of file %s", blockNum, filename);
                write_block(sb, filename, blockNum, buf);
                break;

            default:
                printf("Unknown operation: %c\n", operation);
                break;
        }
    }

    fclose(inputFile);

    printf("\nFinal State of File System:\n");
    print_file_system(sb);

    return 0;
}


void delete_file(struct superblock *sb, const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (sb->inodes[i].used && strcmp(sb->inodes[i].name, filename) == 0) {
            // Mark corresponding blocks as free
            for (int j = 0; j < sb->inodes[i].size; j++) {
                int block_index = sb->inodes[i].blockPointers[j];
                sb->free_blocks[block_index] = 0;
            }

            // Mark the inode as free
            sb->inodes[i].used = 0;
            printf("File %s deleted.\n", filename);
            return;
        }
    }

    printf("File %s not found.\n", filename);
}

void read_block(struct superblock *sb, const char *filename, int blockNum, char *buf) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (sb->inodes[i].used && strcmp(sb->inodes[i].name, filename) == 0) {
            if (blockNum >= 0 && blockNum < sb->inodes[i].size) {
                int block_index = sb->inodes[i].blockPointers[blockNum];
                printf("Reading block %d from file %s:\n", blockNum, filename);
                printf("%s\n", &disk[block_index * BLOCK_SIZE]);
                return;
            } else {
                printf("Invalid block number for file %s.\n", filename);
                return;
            }
        }
    }

    printf("File %s not found.\n", filename);
}

void write_block(struct superblock *sb, const char *filename, int blockNum, const char *buf) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (sb->inodes[i].used && strcmp(sb->inodes[i].name, filename) == 0) {
            if (blockNum >= 0 && blockNum < sb->inodes[i].size) {
                int block_index = sb->inodes[i].blockPointers[blockNum];
                printf("Writing to block %d of file %s:\n", blockNum, filename);
                strncpy(&disk[block_index * BLOCK_SIZE], buf, BLOCK_SIZE);
                printf("%s\n", buf);
                return;
            } else {
                printf("Invalid block number for file %s.\n", filename);
                return;
            }
        }
    }

    printf("File %s not found.\n", filename);
}

void list_files(struct superblock *sb) {
    printf("Files in File System:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (sb->inodes[i].used) {
            printf("File %d: %s, Size: %d blocks\n", i + 1, sb->inodes[i].name, sb->inodes[i].size);
        }
    }
    printf("\n");
}
