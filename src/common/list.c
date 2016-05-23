#include <stdlib.h>
#include "list.h"

void list_add(list_node ** head, void * element)
{
    if (*head == NULL) {

        *head = malloc(sizeof(list_node));
        (*head)->element = element;
        (*head)->next = NULL;

        return;
    }

    list_node * current = *head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(list_node));
    current->next->element = element;
    current->next->next = NULL;
}

void list_foreach(list_node * head, void (* action)(void *))
{
    if (action == NULL) return;

    for (list_node * current = head; current != NULL; current = current->next) {
        (*action)(current->element);
    }
}

list_node * list_map(list_node * src_list, void * (* mapper)(void *))
{
    if (mapper == NULL) {
        return src_list;
    }

    list_node * new_list = NULL;

    for (list_node * current = src_list; current != NULL; current = current->next) {
        list_add(&new_list, (*mapper)(current->element));
    }

    return new_list;
}

void list_destroy(list_node ** head, void (* element_destructor)(void **))
{
    for (list_node * current = *head; current != NULL; current = current->next) {
        list_node * temp = current;

        if (element_destructor != NULL) {
            (*element_destructor)(&temp->element);
        }

        free(temp);
    }

    *head = NULL;
}