#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "connection_handler.h"
#include "../utils/utils.h"
#include "../results/results.h"
#include "../../common/questions.h"

void * handle_connection(void * holder)
{
    //region Dereference holder
    int sd = *((client_handler_data_holder *) holder)->client_socket_descriptor;
    struct sockaddr_in * client = ((client_handler_data_holder *) holder)->client;
    list_node * question_pool = ((client_handler_data_holder *) holder)->question_pool;
    list_node * results = ((client_handler_data_holder *) holder)->results;
    //endregion

    char * server_msg;
    char client_msg[MESSAGE_BUFFER];

    //region Client specific data
    long faculty_id = -1;
    int points = -1;

    list_node * client_questions = list_pick_random(question_pool, QUESTIONS_PER_USER);
    list_node * client_questions_list_current = NULL;
    question * current_question = NULL;
    //endregion

    server_write(sd, "SEND_ID");

    long bytes_read;
    while ((bytes_read = recv(sd, client_msg, MESSAGE_BUFFER, 0)) > 0)
    {
        client_msg[bytes_read] = 0; // replace the \n with NUL-terminator

        if (faculty_id == -1)
        {
            int read_faculty_id = sscanf(client_msg, "%li", &faculty_id);

            if (read_faculty_id != 1)
            {
                server_write(sd, "SEND_ID");
                continue;
            }
            else
            {
                char test_taken = result_list_has_faculty_id(results, faculty_id);

                if (test_taken)
                {
                    faculty_id = -1;
                    server_write(sd, "TEST_TAKEN");
                    break;
                }

                points = 0;
                server_write(sd, "OK");
                continue;
            }
        }

        if (!strcmp(client_msg, "NEXT_QUESTION"))
        {
            if (client_questions_list_current == NULL)
            {
                client_questions_list_current = client_questions;
                current_question = client_questions_list_current->element;
            }
            else if (client_questions_list_current->next != NULL)
            {
                client_questions_list_current = client_questions->next;
                current_question = client_questions_list_current->element;
            }
            else
            {
                server_write(sd, "END");
                continue;
            }

            server_msg = question_to_string_for_user(current_question);
            server_write(sd, server_msg);
            free(server_msg);

            continue;
        }

        //region Receive answer, check if correct and store points
        if (strncmp(client_msg, "ANSWER_", strlen("ANSWER_")) == 0)
        {
            int answer;
            sscanf(client_msg, "ANSWER_%d", &answer);

            if (current_question->correct_answer_index == answer - 1)
            {
                points++;
            }

            server_write(sd, "OK");
            continue;
        }
        //endregion

        if (!strcmp(client_msg, "GET_RESULTS"))
        {
            char points_str[100];
            sprintf(points_str, "%d", points);
            server_write(sd, points_str);

            break;
        }
    }

    if (faculty_id > 0 && points >= 0)
    {
        list_add(&results, result_create(faculty_id, points));
    }

    //region Disconnect handling
    if (bytes_read >= 0)
    {
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client->sin_addr, ip_str, INET_ADDRSTRLEN);
        printf("Client (%s) disconnected\n", ip_str);
    }
    else if (bytes_read == -1)
    {
        perror("recv");
    }

    close(sd);
    destroy_holder(holder);
    //endregion

    return NULL;
}