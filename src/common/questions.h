#pragma once

#define ANSWERS_PER_QUESTION 4
#define QUESTIONS_PER_USER 10

typedef struct
{
    char * text;
    char * answers[ANSWERS_PER_QUESTION];
    int correct_answer_index;
} question;

question * question_create(char * text);

void question_add_answer(question * q, char * answer, char is_correct);

char * question_to_string_for_user(question * q);

void question_delete(question ** q);