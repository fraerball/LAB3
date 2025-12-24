#pragma once

#include <stddef.h>

typedef struct container container_t;

typedef int (*comparator_t)(const void* publiication1, const void* publication2);

void sort(container_t* container, comparator_t cmp);