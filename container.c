#include"container.h"

typedef struct stack_node{
    publication_t data;
    struct stack_node* next;
} stack_node_t;

struct container {
    stack_node_t* top;
    size_t size;
};

struct container_iterator {
    const container_t* container;
    stack_node_t* current;
    size_t position;
};

container_t* container_init() {
    container_t* container = malloc(sizeof(container_t));
    if (container == NULL) return NULL;
    
    container->top = NULL;
    container->size = 0;
    return container;
}
