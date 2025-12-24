#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"

static int set_mode(const char* arg, mode_t* mode){
    if (strcmp(arg, "--generate") == 0 || strcmp(arg, "-g") == 0){
        *mode = MODE_GENERATE;
        return 1;
    }
    if (strcmp(arg, "--sort") == 0 || strcmp(arg, "-s") == 0){
        *mode = MODE_SORT;
        return 1;
    }
    if (strcmp(arg, "--print") == 0 || strcmp(arg, "-P") == 0){
        *mode = MODE_PRINT;
        return 1;
    }
    return 0;
}

static int set_sort_type(const char* arg, sort_type_t* type){
    if (strcmp(arg, "asc") == 0 || strcmp(arg, "A") == 0){
        *type = SORT_ASC;
        return 1;
    }
    if (strcmp(arg, "desc") == 0 || strcmp(arg, "D") == 0){
        *type = SORT_DESC;
        return 1;
    }
    return 0;
}
static int set_sort_view(const char* arg, sort_view_t* view){
    if (strcmp(arg, "year") == 0 || strcmp(arg, "y") == 0) {
        *view = SORT_YEAR;
        return 1;
    }   
     if (strcmp(arg, "citations") == 0 || strcmp(arg, "c") == 0){
        *view = SORT_CITATIONS;
        return 1;
    }   
}

int analysis_args(int argc, char* argp[], program_args_t* args){

    args->mode = MODE_NONE;
    args->sort_type = SORT_ASC;
    args->sort_view = SORT_YEAR;
    args->generate_quantity = 0;
    args->input_file = NULL;
    args->output_file = NULL;
    args->surnames_file = NULL;
    args->words_file = NULL;
    args->initials_file = NULL;
    
    for (int i = 1; i < argc; i++){
        if (set_mode(argp[i], &args->mode)){
            continue;
        }
        if (strncmp(argp[i], "--in=", 5) == 0){
            args->input_file = argp[i] + 5;
        }
        else if (strcmp(argp[i], "-i") == 0 && i + 1 < argc){
            args->input_file = argp[++i];
        }
        else if (strncmp(argp[i], "--out=", 6) == 0){
            args->output_file = argp[i] + 6;
        }
        else if (strcmp(argp[i], "-o") == 0 && i + 1 < argc){
            args->output_file = argp[++i];
        }
        else if (strncmp(argp[i], "--surnames=", 11) == 0){
            args->surnames_file = argp[i] + 11;
        }
        else if (strcmp(argp[i], "-S") == 0 && i + 1 < argc){
            args->surnames_file = argp[++i];
        }
        else if (strncmp(argp[i], "--words=", 8) == 0){
            args->words_file = argp[i] + 8;
        }
        else if (strcmp(argp[i], "-w") == 0 && i + 1 < argc){
            args->words_file = argp[++i];
        }
        else if (strncmp(argp[i], "--initials=", 11) == 0){
            args->initials_file = argp[i] + 11;
        }
        else if (strcmp(argp[i], "-I") == 0 && i + 1 < argc){
            args->initials_file = argp[++i];
        }
        else if (strncmp(argp[i], "--type=", 7) == 0){
            set_sort_type(argp[i] + 7, &args->sort_type);
        }
        else if (strcmp(argp[i], "-t") == 0 && i + 1 < argc){
            set_sort_type(argp[++i], &args->sort_type);
        }
        else if (strncmp(argp[i], "--view=", 7) == 0){
            set_sort_view(argp[i] + 8, &args->sort_view);
        }
        else if (strcmp(argp[i], "-v") == 0 && i + 1 < argc){
            set_sort_view(argp[++i], &args->sort_view);
        }
        else if (args->mode == MODE_GENERATE && args->generate_quantity == 0){
            args->generate_quantity = atoi(argp[i]);
        }
        else {
            fprintf(stderr, "Ошибка: %s\n", argp[i]);
            return 0;
        }
    }
}