#include"container.h"

typedef struct stack_node{
    publication_t data;
    struct stack_node* next;
} stack_node_t;

struct container{
    stack_node_t* top;
    size_t size;
};

struct container_iterator{
    const container_t* container;
    stack_node_t* current;
    size_t position;
};

container_t* container_init(){
    container_t* container = malloc(sizeof(container_t));
    if (container == NULL) return NULL;
    
    container->top = NULL;
    container->size = 0;
    return container;
}

void container_destroy(container_t* container){
    if (container == NULL) return NULL;
    
    container_clear(container);
    free(container);
}
int container_pop(container_t* container){
    if (container == NULL || container->top == NULL) return 0;
    
    stack_node_t* temp = container->top;
    container->top = container->top->next;
    
    publication_free(&temp->data);
    free(temp);
    container->size--;
    
    return 1;
}
int container_push(container_t* container, const publication_t* data){
    if (container == NULL || data == NULL) return 0;
    
    stack_node_t* new_node = malloc(sizeof(stack_node_t));
    if (new_node == NULL) return 0;
    
    if (publication_copy(&new_node->data, data) == 0){
        free(new_node);
        return 0;
    }
}