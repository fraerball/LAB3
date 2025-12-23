#pragma once

#include <stddef.h>
#include <stdlib.h>

typedef struct {
    char* title_publication;
    char* author_surname;
    char* author_initials;
    char* title_journal;
    unsigned short publication_year;
    unsigned short journal_volume;
    unsigned char is_rinz;
    unsigned short page_quantity;
    unsigned short citation_quantity;
} publication_t;

int publication_copy(publication_t* dest, const publication_t* src);
void publication_free(publication_t* publication);
