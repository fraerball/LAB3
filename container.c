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

void container_clear(container_t* container){
    if (container == NULL) return;
    
    while (container->top){
        container_pop(container);
    }
}
int container_paste(container_t* container, size_t index, const publication_t* data){
    if (container == NULL || data == NULL|| index > container->size) return 0;
    
    if (index == container->size){
        return container_push(container, data);
    }

    container_t* temp_container = container_init();
    if (temp_container == NULL) return 0;
    
    for (size_t i = container->size; i > index; i--){
        publication_t temp_publication;
        if (container_get(container, i - 1, &temp_publication)){
            container_push(temp_container, &temp_publication);
            publication_free(&temp_publication);
        }
    }
    
    if (!container_push(container, data)){
        container_destroy(temp_container);
        return 0;
    }

    while (container_size(temp_container) > 0){
        publication_t temp_publication;
        if (container_get(temp_container, 0, &temp_publication)){
            container_push(container, &temp_publication);
            container_pop(temp_container);
            publication_free(&temp_publication);
        }
    }
    
    container_destroy(temp_container);
    return 1;
}

int container_remove(container_t* container, size_t index){
    if (container == NULL || index >= container->size) return 0;
    
    if (index == container->size - 1){
        return container_pop(container);
    }
    
    container_t* temp_container = container_init();
    if (temp_container == NULL) return 0;
    
    for (size_t i = container->size - 1; i > index; i--){
        publication_t temp_publication;
        if (container_get(container, i, &temp_publication)){
            container_push(temp_container, &temp_publication);
            publication_free(&temp_publication);
        }
    }
    
    container_pop(container);
    
    while (container_size(temp_container) > 0){
        publication_t temp_publication;
        if (container_get(temp_container, 0, &temp_publication)){
            container_push(container, &temp_publication);
            container_pop(temp_container);
            publication_free(&temp_publication);
        }
    }
    
    container_destroy(temp_container);
    return 1;
}

int container_swap(container_t* container, size_t index1, size_t index2){
    if (container == NULL || index1 >= container->size || index2 >= container->size) return 0;
    
    if (index1 == index2) return 1;
    
    publication_t publication1 , publication2;
    if (container_get(container, index1, &publication1) == 0 || container_get(container, index2, &publication2) == 0){
        return 0;
    }
    
    int result = container_update(container, index1, &publication2) && container_update(container, index2, &publication1);
    
    publication_free(&publication1);
    publication_free(&publication2);
    
    return result;
}
size_t container_size(const container_t* container){
    if (container == NULL) {
        return 0;
    }
    return container->size;
}
int container_get(const container_t* container, size_t index, publication_t* result){
    if (container == NULL || result == NULL || index >= container->size) return 0;
    
    stack_node_t* node = get_node_index(container, index);
    if (node == NULL) return 0;
    
    return publication_copy(result, &node->data);
}
static stack_node_t* get_node_index(const container_t* container, size_t index){
    if (index >= container->size) return NULL;
    
    stack_node_t* current = container->top;
    for (size_t i = 0; i < container->size - index - 1; i++) {
        current = current->next;
    }
    return current;
}
int container_update(container_t* container, size_t index, const publication_t* data){
    if (!container || !data || index >= container->size) return 0;
    
    stack_node_t* node = get_node_index(container, index);
    if (!node) return 0;
    
    publication_free(&node->data);
    return publication_copy(&node->data, data);
}