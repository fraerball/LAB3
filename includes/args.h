#pragma once

#include <stddef.h>

typedef enum{
    MODE_NONE,
    MODE_GENERATE,
    MODE_SORT,
    MODE_PRINT
} mode_t;

typedef enum{
    SORT_YEAR,
    SORT_CITATIONS
} sort_view_t;

typedef enum{
    SORT_ASC,
    SORT_DESC
} sort_type_t;

typedef struct{
    mode_t mode;
    sort_type_t sort_type;
    sort_view_t sort_view;
    int generate_quantity;
    char* input_file;
    char* output_file;
    char* surnames_file;
    char* words_file;
    char* initials_file;
} program_args_t;

int analysis_args(int argc, char* argp[], program_args_t* args);
