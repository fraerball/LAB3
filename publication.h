#pragma once

#include <stddef.h>

typedef struct {
    char* title;
    char* author_surname;
    char* author_initials;
    char* journal_name;
    unsigned short publication_year;
    unsigned short journal_volume;
    int is_rinz;
    int page_count;
    int citation_count;
} publication_t;

int publication_copy(publication_t* dest, const publication_t* src);
void publication_free(publication_t* pub);