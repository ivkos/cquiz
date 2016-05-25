#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "../connection_handler/connection_handler.h"
#include "utils.h"

client_handler_data_holder * create_holder(int client_socket_descriptor, struct sockaddr_in * client,
                                           list_node * question_pool, list_node * results)
{
    client_handler_data_holder * holder = malloc(sizeof(client_handler_data_holder));

    holder->client_socket_descriptor = malloc(sizeof(int));
    *holder->client_socket_descriptor = client_socket_descriptor;

    holder->client = client;
    holder->question_pool = question_pool;
    holder->results = results;

    return holder;
}

void destroy_holder(client_handler_data_holder * holder)
{
    free(holder->client_socket_descriptor);
    free(holder);
}

struct sockaddr_in server_create_sin(unsigned short port)
{
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    return server;
}

int server_create_socket()
{
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_descriptor == -1)
    {
        perror("Could not create socket");
        exit(1);
    }

    //region Mark socket as reusable to prevent binding issues
    {
        int opt_val = 1;
        socklen_t opt_len = sizeof(opt_val);
        setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &opt_val, opt_len);
    }
    //endregion

    return socket_descriptor;
}

void server_listen(int server_sd, struct sockaddr_in * server)
{
    int bind_result = bind(
            server_sd,
            (const struct sockaddr *) server,
            sizeof(*server)
    );

    if (bind_result < 0)
    {
        perror("Could not bind");
        exit(1);
    }

    listen(server_sd, 100);
}

void server_start(unsigned short port, int * server_socket_fd, list_node * question_pool, list_node * results)
{
    struct sockaddr_in server = server_create_sin(port);
    int server_sd = server_create_socket();
    *server_socket_fd = server_sd;
    server_listen(server_sd, &server);

    struct sockaddr_in client;
    int client_sd;
    socklen_t client_socket_len = sizeof(struct sockaddr_in);

    printf("Listening for connections on port %d...\n", port);

    while ((client_sd = accept(server_sd, (struct sockaddr *) &client, &client_socket_len)))
    {
        client_handler_data_holder * holder = create_holder(client_sd, &client, question_pool, results);

        //region Log connection
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, ip_str, INET_ADDRSTRLEN);
        printf("Client (%s) connected\n", ip_str);
        //endregion

        pthread_t thread;
        int res = pthread_create(&thread, NULL, handle_connection, holder);

        if (res < 0)
        {
            perror("Could not create client handler thread");
            return;
        }

        pthread_detach(thread);
    }
}

long server_write(int socket_fd, char * buf)
{
    return write(socket_fd, buf, strlen(buf));
}