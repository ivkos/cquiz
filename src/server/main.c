#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <signal.h>
#include "question_parser/question_parser.h"
#include "results/results.h"
#include "server_utils/server_utils.h"
#include "../common/questions.h"

list_node * questions_pool;
list_node * results;

void goodbye(int signum)
{
    results_write_list_to_file("results.txt", results);

    list_destroy(&results, result_delete);
    list_destroy(&questions_pool, question_delete);

    exit(0);
}

int main(int argc, char ** argv)
{
    if (argc >= 2 && (strcmp("--help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0))
    {
        printf("Usage: %s [questions] [port]\n", basename(argv[0]));
        printf("Default: %s questions.md 3000\n", basename(argv[0]));
        exit(0);
    }

    char * file = argc >= 2 ? argv[1] : "questions.md";
    unsigned short port = (unsigned short) (argc >= 3 ? atoi(argv[2]) : 3000);

    questions_pool = questions_parse_file(file);
    results = results_parse_file("results.txt");

    signal(SIGINT, goodbye);
    signal(SIGTERM, goodbye);

    server_start(port, questions_pool, results);

    return 0;
}
