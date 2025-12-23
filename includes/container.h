#pragma once
#include "publication.h"

typedef struct container container_t;

container_t* container_init();
void container_destroy(container_t* container);
size_t container_size(const container_t* container);
int container_get(const container_t* container, size_t index, publication_t* result);
int container_update(container_t* container, size_t index, const publication_t* data);
int container_push(container_t* container, const publication_t* data);
int container_pop(container_t* container);
int container_paste(container_t* container, size_t index, const publication_t* data);
int container_remove(container_t* container, size_t index);
int container_swap(container_t* container, size_t index1, size_t index2);
void container_clear(container_t* container);

typedef struct container_iterator container_iterator_t;

container_iterator_t* container_iterator_create(const container_t* container);
void container_iterator_destroy(container_iterator_t* iterator);
int container_iterator_next(container_iterator_t* iterator);
int container_iterator_has_next(const container_iterator_t* iterator);
int container_iterator_get(const container_iterator_t* iterator, publication_t* result);
int container_iterator_is_valid(const container_iterator_t* iterator);