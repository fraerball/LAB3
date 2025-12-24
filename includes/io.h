#pragma once

#include "container.h"

container_t* read_publications_csv(const char* filename);
int write_publications_csv(const container_t* container, const char* filename);
int print_publications_table(const container_t* container, const char* filename);

container_t* generate_random_publications(int quantity,  const char* surnames_file, const char* words_file, const char* initials_file);

char** read_surnames_from_file(const char* filename, int* quantity);
void free_surnames_array(char** surnames, int quantity);