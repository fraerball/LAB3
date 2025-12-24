#include "publication.h"
#include <stdlib.h>
#include <string.h>

static char* copy_string(const char* str) {
    if (!str) return NULL;
    char* new_str = malloc(strlen(str) + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

int publication_copy(publication_t* dest, const publication_t* src) {
    if (!src || !dest) return 0;
    
    dest->title_publication = copy_string(src->title_publication);
    dest->author_surname = copy_string(src->author_surname);
    dest->author_initials = copy_string(src->author_initials);
    dest->title_journal = copy_string(src->title_journal);
    
    dest->publication_year = src->publication_year;
    dest->journal_volume = src->journal_volume;
    dest->is_rinz = src->is_rinz;
    dest->page_quantity = src->page_quantity;
    dest->citation_quantity = src->citation_quantity;
    
    return 1;
}

void publication_free(publication_t* publication) {
    if (!publication) return;
    
    free(publication->title_publication);
    free(publication->author_surname);
    free(publication->author_initials);
    free(publication->title_journal);
    
    publication->title_publication = NULL;
    publication->author_surname = NULL;
    publication->author_initials = NULL;
    publication->title_journal = NULL;
}

int cmp_year_asc(const void* value1, const void* value2) {
    const publication_t* publication1 = (const publication_t*)value1;
    const publication_t* publication2 = (const publication_t*)value2;
    
    if (publication1->publication_year != publication2->publication_year) {
        return publication1->publication_year - publication2->publication_year;
    }
    return publication1->citation_quantity - publication2->citation_quantity;
}


int cmp_year_desc(const void* value1, const void* value2){
    return -cmp_year_asc(value1, value2);
}


int cmp_cit_asc(const void* value1, const void* value2){
    const publication_t* publication1 = (const publication_t*)value1;
    const publication_t* publication2 = (const publication_t*)value2;
    
    if (publication1->citation_quantity != publication2->citation_quantity) {
        return publication1->citation_quantity - publication2->citation_quantity;
    }
    return publication1->publication_year - publication2->publication_year;
}

int cmp_cit_desc(const void* value1, const void* value2){
    return -cmp_cit_asc(value1, value2);
}