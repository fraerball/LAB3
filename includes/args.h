#pragma once

#include <stddef.h>

typedef enum {
    MODE_NONE,
    MODE_GENERATE,
    MODE_SORT,
    MODE_PRINT
} mode_t;

typedef enum {
    SORT_FIELD_YEAR,
    SORT_FIELD_CITATIONS
} sort_field_t;

typedef enum {
    SORT_ASC,
    SORT_DESC
} sort_type_t;

typedef struct {
    mode_t mode;
    sort_type_t sort_type;
    sort_field_t sort_field;
    int generate_count;
    char* input_file;
    char* output_file;
    char* surnames_file;
    char* words_file;
    char* initials_file;
} program_args_t;

int analysis_args(int argc, char* argv[], program_args_t* args);
