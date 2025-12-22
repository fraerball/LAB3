#pragma once
#include "publication.h"

typedef struct container container_t;

container_t* container_create(void);
void container_destroy(container_t* container);
int container_add(container_t* container, const publication_t* pub);
int container_get_count(const container_t* container);