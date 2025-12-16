#pragma once

#include <stddef.h>

typedef enum {
    MODE_NONE = 0,
    MODE_GENERATE,
    MODE_SORT,
    MODE_PRINT
} program_mode_t;

typedef enum {
    SORT_FIELD_NONE = 0,
    SORT_FIELD_YEAR,
    SORT_FIELD_CITATIONS
} sort_field_t;

typedef enum {
    SORT_ORDER_NONE = 0,
    SORT_ORDER_ASC,
    SORT_ORDER_DESC
} sort_order_t;

typedef struct {
    program_mode_t mode;

    char* input_file;
    char* output_file;

    size_t generate_count;

    sort_field_t sort_field;
    sort_order_t sort_order;
} program_args_t;

void args_init(program_args_t* args);

void args_free(program_args_t* args);

int parse_args(int argc, char* argv[], program_args_t* args);
