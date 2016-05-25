#pragma once

#include "../../common/list.h"

typedef struct client_handler_data_holder
{
    int * client_socket_descriptor;
    struct sockaddr_in * client;
    list_node * question_pool;
    list_node * results;
} client_handler_data_holder;

client_handler_data_holder * create_holder(int client_socket_descriptor, struct sockaddr_in * client,
                                           list_node * question_pool, list_node * results);

void destroy_holder(client_handler_data_holder * holder);

struct sockaddr_in server_create_sin(unsigned short port);

int server_create_socket();

void server_listen(int server_sd, struct sockaddr_in * server);

void server_start(unsigned short port, list_node * question_pool, list_node * results);

long server_write(int socket_fd, char * buf);