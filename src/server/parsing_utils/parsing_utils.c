#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "parsing_utils.h"

int parsing_open_file(char * file_path, int flags)
{
    int fd = open(file_path, flags);

    if (fd == -1)
    {
        fprintf(stderr, "Could not open file '%s': %s\n", file_path, strerror(errno));

        exit(1);
    }

    return fd;
}

char * parsing_allocate_buffer(int fd)
{
    long size = lseek(fd, 0, SEEK_END);
    long s = lseek(fd, 0, SEEK_SET);

    if (size == -1 || s == -1)
    {
        perror("seek");
        exit(1);
    }

    char * buf = malloc((size + 1) * sizeof(char));

    if (buf == NULL)
    {
        fputs("Could not allocate buffer for file", stderr);
        exit(1);
    }

    return buf;
}