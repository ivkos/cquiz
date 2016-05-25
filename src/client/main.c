#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/connector.h"

int main(int argc, char ** argv)
{
    if (argc < 2 || (argc == 2 && (strcmp("--help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0)))
    {
        printf("Usage: %s <host> [port]\n", basename(argv[0]));
        printf("Default: %s <host> 3000\n", basename(argv[0]));

        exit(0);
    }

    char * hostname = argv[1];
    unsigned short port = (unsigned short) (argc >= 3 ? atoi(argv[2]) : 3000);

    int socket_fd = client_create_socket(hostname, port);
    interact(socket_fd);
    close(socket_fd);

    return 0;
}