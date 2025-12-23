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