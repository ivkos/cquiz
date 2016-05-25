#include <stdlib.h>
#include <time.h>
#include "list.h"

void list_add(list_node ** head, void * element)
{
    if (*head == NULL)
    {

        *head = malloc(sizeof(list_node));
        (*head)->element = element;
        (*head)->next = NULL;

        return;
    }

    list_node * current = *head;

    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = malloc(sizeof(list_node));
    current->next->element = element;
    current->next->next = NULL;
}

void * list_get_element_at(list_node * head, int index)
{
    int current_index = 0;
    void * element = NULL;

    for (list_node * current = head; current != NULL; current = current->next)
    {
        if (current_index == index)
        {
            element = current->element;
            break;
        }

        ++current_index;
    }

    return element;
}

int list_count(list_node * head)
{
    int count = 0;

    for (list_node * current = head; current != NULL; current = current->next)
    {
        count++;
    }

    return count;
}

list_node * list_pick_random(list_node * src_list, int count)
{
    int src_list_count = list_count(src_list);

    if (count >= src_list_count) return src_list;

    int indexes_to_pick[count];
    for (int i = 0; i < count; ++i)
        indexes_to_pick[i] = -1;

    srand((unsigned int) time(NULL));

    for (int i = 0; i < count;)
    {
        int random_index = rand() % src_list_count;

        char exists = 0;
        for (int j = 0; j < i; ++j)
        {
            if (indexes_to_pick[j] == random_index)
            {
                exists = 1;
                break;
            }
        }

        if (!exists)
        {
            indexes_to_pick[i] = random_index;
            ++i;
        }
        else
        {
            continue;
        }
    }

    list_node * new_list = NULL;

    for (int i = 0; i < count; ++i)
    {
        list_add(&new_list, list_get_element_at(src_list, indexes_to_pick[i]));
    }

    return new_list;
}

void list_destroy(list_node ** head, void (* element_destructor)(void **))
{
    for (list_node * current = *head; current != NULL; current = current->next)
    {
        list_node * temp = current;

        if (element_destructor != NULL)
        {
            (*element_destructor)(&temp->element);
        }

        free(temp);
    }

    *head = NULL;
}