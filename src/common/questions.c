#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "questions.h"

question * question_create(char * text)
{
    question * q = malloc(sizeof(question));

    q->text = malloc(strlen(text) + 1);
    strcpy(q->text, text);

    for (int i = 0; i < ANSWERS_PER_QUESTION; ++i) {
        q->answers[i] = NULL;
    }

    q->correct_answer_index = -1;

    return q;
}

void question_add_answer(question * q, char * answer, char is_correct)
{
    for (int i = 0; i < ANSWERS_PER_QUESTION; ++i) {
        if (q->answers[i] == NULL) {
            q->answers[i] = malloc(strlen(answer) + 1);
            strcpy(q->answers[i], answer);

            if (is_correct) q->correct_answer_index = i;

            break;
        }
    }
}

char * question_to_string_for_user(question * q)
{
    char * str = malloc(sizeof(char) * 2000);

    strcpy(str, q->text);

    strcpy(str + strlen(str), "\n");

    for (int i = 0; i < ANSWERS_PER_QUESTION; ++i) {
        sprintf(str + strlen(str), "[%d] %s\n", i + 1, q->answers[i]);
    }

    strcpy(str + strlen(str), "\n");

    return str;
}

void question_print(question * q)
{
    puts(q->text);

    for (int i = 0; i < ANSWERS_PER_QUESTION; ++i) {
        printf("[%c] %s\n", q->correct_answer_index == i ? 'x' : ' ', q->answers[i]);
    }

    putchar('\n');
}

void question_delete(question ** q)
{
    free((*q)->text);
    (*q)->text = NULL;

    for (int i = 0; i < ANSWERS_PER_QUESTION; ++i) {
        free((*q)->answers[i]);
        (*q)->answers[i] = NULL;
    }

    (*q)->correct_answer_index = -1;

    free(*q);
    *q = NULL;
}