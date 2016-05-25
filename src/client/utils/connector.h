#pragma once
#define BUFFER_LENGTH 2000


int client_create_socket(char * hostname, unsigned short port);

void clear_screen();

void interact(int socket_fd);