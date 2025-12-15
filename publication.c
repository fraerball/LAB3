#include "publication.h"
#include <stdlib.h>
#include <string.h>

static char* safe_strdup(const char* str) {
    if (!str) return NULL;
    char* new_str = malloc(strlen(str) + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

int publication_copy(publication_t* dest, const publication_t* src) {
    if (!src || !dest) return 0;
    
    dest->title = safe_strdup(src->title);
    dest->author_surname = safe_strdup(src->author_surname);
    dest->author_initials = safe_strdup(src->author_initials);
    dest->journal_name = safe_strdup(src->journal_name);
    
    dest->publication_year = src->publication_year;
    dest->journal_volume = src->journal_volume;
    dest->is_rinz = src->is_rinz;
    dest->page_count = src->page_count;
    dest->citation_count = src->citation_count;
    
    return 1;
}

void publication_free(publication_t* pub) {
    if (!pub) return;
    
    free(pub->title);
    free(pub->author_surname);
    free(pub->author_initials);
    free(pub->journal_name);
    
    pub->title = NULL;
    pub->author_surname = NULL;
    pub->author_initials = NULL;
    pub->journal_name = NULL;
}