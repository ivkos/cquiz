typedef struct list_node
{
    void * element;
    struct list_node * next;
} list_node;


void list_add(list_node ** head, void * element);

void list_foreach(list_node * head, void (* action)(void *));

list_node * list_map(list_node * src_list, void * (* mapper)(void *));

void list_destroy(list_node ** head, void (* element_destructor)(void **));