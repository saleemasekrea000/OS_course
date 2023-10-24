#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>
#include <stdio.h>
#define FILE_SIZE (500 * 1024 * 1024) // 500 MiB

int main()
{
    int text_fd = open("text.txt", O_CREAT | O_WRONLY, 0666);
    if (text_fd == -1)
    {
        perror("open");
        exit(1);
    }
    int random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1)
    {
        perror("open");
        close(text_fd);
        exit(1);
    }
    char buffer[1024];
    char *text = (char *)malloc(FILE_SIZE);
    size_t text_length = 0;

    while (text_length < FILE_SIZE)
    {
        ssize_t n = read(random_fd, buffer, sizeof(buffer));
        if (n == -1)
        {
            perror("read");
            close(random_fd);
            close(text_fd);
            free(text);
            exit(1);
        }

        for (ssize_t i = 0; i < n && text_length < FILE_SIZE; i++)
        {
            if (isprint(buffer[i]))
            {
                text[text_length] = buffer[i];
                text_length++;
                if (text_length % 1024 == 0)
                {
                    text[text_length] = '\n';
                    text_length++;
                }
            }
        }
    }
    ssize_t written = write(text_fd, text, text_length);
    if (written == -1)
    {
        perror("write");
        close(random_fd);
        close(text_fd);
        free(text);
        exit(1);
    }
    close(random_fd);
    close(text_fd);
    text_fd = open("text.txt", O_RDWR);
    if (text_fd == -1)
    {
        perror("open");
        free(text);
        exit(1);
    }
    long page_size = sysconf(_SC_PAGESIZE);
    off_t offset = 0;
    char *mapped;
    size_t total_caps = 0;
    while (offset < text_length)
    {
        size_t chunk_size = page_size * 1024;
        if (text_length - offset < chunk_size)
        {
            chunk_size = text_length - offset;
        }

        mapped = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE, MAP_SHARED, text_fd, offset);
        if (mapped == MAP_FAILED)
        {
            perror("mmap");
            close(text_fd);
            free(text);
            exit(1);
        }
        for (size_t i = 0; i < chunk_size; i++)
        {
            if (isupper(mapped[i]))
            {
                mapped[i] = tolower(mapped[i]);
                total_caps++;
            }
        }
        munmap(mapped, chunk_size);
        offset += chunk_size;
    }

    close(text_fd);
    printf("Total capital letters: %zu\n", total_caps);
    free(text);
    return 0;
}
