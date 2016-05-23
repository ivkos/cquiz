#pragma once

#include "../common/list.h"

int questions_open_file(char * file_path);

char * questions_allocate_buffer(int fd);

list_node * questions_parse(char * file_path);