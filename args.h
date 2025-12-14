#pragma once

typedef enum {
    MODE_NONE,
    MODE_GENERATE,
    MODE_SORT,
    MODE_PRINT
} mode_t;

typedef enum {
    SORT_ASC,
    SORT_DESC
} sort_type_t;

typedef enum {
    SORT_BY_YEAR,
    SORT_BY_CITATIONS
} sort_field_t;

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