#pragma once

#include "../../common/list.h"

typedef struct
{
    long faculty_id;
    int points;
} result;

result * result_create(long faculty_id, int points);

list_node * results_parse_file(char * file_path);

void results_write_list_to_file(char * file_path, list_node * result_list);

char result_list_has_faculty_id(list_node * result_list, long faculty_id);

void result_delete(result ** r);