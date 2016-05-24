#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "question_parser.h"
#include "../../common/questions.h"
#include "../parsing_utils/parsing_utils.h"

list_node * questions_parse_file(char * file_path)
{
    int fd = parsing_open_file(file_path, O_RDONLY);
    char * buf = parsing_allocate_buffer(fd);
    read(fd, buf, malloc_usable_size(buf));

    question * current_question = NULL;
    list_node * question_list = NULL;

    for (char * context, * token = strtok_r(buf, "\n", &context);
         token != NULL;
         token = strtok_r(NULL, "\n", &context)) {

        char * question_text = malloc(strlen(token));
        int parse_question = sscanf(token, "### %[^\n]", question_text);

        if (parse_question > 0) {
            current_question = question_create(question_text);

            list_add(&question_list, current_question);
        } else {
            char * answer_text = malloc(strlen(token));
            char answer_correct_mark;

            int parse_answer = sscanf(token, "- [%c] %[^\n]", &answer_correct_mark, answer_text);

            if (parse_answer > 0) {
                question_add_answer(current_question, answer_text, answer_correct_mark == 'x');
            }

            free(answer_text);
        }

        free(question_text);
    }

    close(fd);
    free(buf);

    return question_list;
}
