#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include "results.h"
#include "../parsing_utils/parsing_utils.h"

result * result_create(long faculty_id, int points)
{
    result * r = malloc(sizeof(result));

    r->faculty_id = faculty_id;
    r->points = points;

    return r;
}

list_node * results_parse_file(char * file_path)
{
    int fd = parsing_open_file(file_path, O_CREAT | O_RDONLY);
    char * buf = parsing_allocate_buffer(fd);
    read(fd, buf, malloc_usable_size(buf));

    list_node * result_list = NULL;

    for (char * context, * token = strtok_r(buf, "\n", &context);
         token != NULL;
         token = strtok_r(NULL, "\n", &context))
    {

        long faculty_id;
        int points;

        int parse_result = sscanf(token, "%li\t%d", &faculty_id, &points);

        if (parse_result == 2)
        {
            list_add(&result_list, result_create(faculty_id, points));
        }
    }

    close(fd);
    free(buf);

    return result_list;
}

void results_write_list_to_file(char * file_path, list_node * result_list)
{
    int fd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC);

    if (fd == -1)
    {
        perror("Could not open results file for writing");

        exit(1);
    }

    for (list_node * current = result_list; current != NULL; current = current->next)
    {
        result * r = (result *) current->element;

        char str[200];
        sprintf(str, "%li\t%d\n", r->faculty_id, r->points);

        write(fd, str, strlen(str));
    }

    close(fd);
}

char result_list_has_faculty_id(list_node * result_list, long faculty_id)
{
    for (list_node * current = result_list; current != NULL; current = current->next)
    {
        if (((result *) current->element)->faculty_id == faculty_id)
        {
            return 1;
        }
    }

    return 0;
}

void result_delete(result ** r)
{
    free(*r);
    *r = NULL;
}