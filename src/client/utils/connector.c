#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "connector.h"
#include "../../common/questions.h"

int client_create_socket(char * hostname, unsigned short port)
{
    int socket_fd;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    struct hostent * server_host;
    server_host = gethostbyname(hostname);

    if (server_host == NULL)
    {
        perror("Host resolution failed");
        exit(1);
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("Cannot initialize socket");
        exit(1);
    }

    bcopy(server_host->h_addr,
          (char *) &server_addr.sin_addr.s_addr,
          server_host->h_length);

    if (connect(socket_fd, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Cannot connect to server");
        exit(1);
    }

    return socket_fd;
}

void clear_screen()
{
    const char * CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";

    write(STDOUT_FILENO, CLEAR_SCREE_ANSI, strlen(CLEAR_SCREE_ANSI));
}


void interact(int socket_fd)
{
    char * client_msg;
    char * server_msg = malloc(sizeof(char) * BUFFER_LENGTH);

    long bytes_read;


    char game_over = 0;
    int questions_answered = 0;

    while ((bytes_read = read(socket_fd, server_msg, BUFFER_LENGTH)) > 0)
    {
        server_msg[bytes_read] = '\0';

        //region SEND_ID -> send faculty_id
        if (strcmp(server_msg, "SEND_ID") == 0)
        {
            while (1)
            {
                clear_screen();
                printf("Enter your faculty ID: ");

                long faculty_id;
                fflush(stdin);
                int scanf_result = scanf("%li", &faculty_id);

                if (scanf_result != 1) continue;

                client_msg = malloc(sizeof(char) * BUFFER_LENGTH);
                sprintf(client_msg, "%li", faculty_id);
                write(socket_fd, client_msg, strlen(client_msg));
                free(client_msg);
                break;
            }

            continue;
        }
        //endregion

        //region TEST_TAKEN -> exit
        if (strcmp(server_msg, "TEST_TAKEN") == 0)
        {
            clear_screen();
            puts("You've already taken the test!");
            break;
        }
        //endregion

        //region OK -> request NEXT_QUESTION
        if (strcmp(server_msg, "OK") == 0)
        {
            client_msg = "NEXT_QUESTION";
            write(socket_fd, client_msg, strlen(client_msg));

            continue;
        }
        //endregion

        //region END -> request GET_RESULTS
        if (strcmp(server_msg, "END") == 0)
        {
            game_over = 1;

            client_msg = "GET_RESULTS";
            write(socket_fd, client_msg, strlen(client_msg));

            continue;
        }
        //endregion

        //region game-over handling
        if (game_over)
        {
            printf("Nice job! You scored %s/%d points\n", server_msg, questions_answered);
            break;
        }
        //endregion

        //region Question/answer
        clear_screen();
        puts(server_msg);

        while (1)
        {
            printf("Your answer: ");

            int answer;
            fflush(stdin);
            int scanf_result = scanf("%d", &answer);

            if (scanf_result != 1 || answer < 1 || answer > ANSWERS_PER_QUESTION) continue;

            client_msg = malloc(sizeof(char) * BUFFER_LENGTH);
            sprintf(client_msg, "ANSWER_%d", answer);
            write(socket_fd, client_msg, strlen(client_msg));
            free(client_msg);

            questions_answered++;

            break;
        }
        //endregion
    }

    free(server_msg);

    if (bytes_read == -1)
    {
        perror("Something broke");
        exit(1);
    }
}
