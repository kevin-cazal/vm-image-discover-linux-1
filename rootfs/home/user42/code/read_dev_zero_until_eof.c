#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
    int fd = open("/dev/zero", O_RDONLY);
    char buffer[BUFFER_SIZE];
    size_t total_read = 0;
    ssize_t bytes_read = 0;

    if (fd < 0)
        return 1;
    while (1) {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == 0)
            break;
        total_read += bytes_read;
    }
    close(fd);
    printf("Done. (Somehow.)\n");
    printf("Total read: %zu bytes.\n", total_read);
    return 0;
}

/*
Note: This code reads from `/dev/zero` until EOF.
For some reason, the code compiles, but it does not work as intended.
Maybe I should delete it, and rethink about what I want to do with this code.
*/