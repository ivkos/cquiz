#include "question_parser.h"
#include "../common/questions.h"

int main()
{
    list_node * questions = questions_parse("questions.txt");

    list_foreach(questions, question_print);

    list_destroy(&questions, question_delete);

    return 0;
}