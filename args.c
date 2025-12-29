#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"

static int set_mode(const char* arg, mode_t* mode){
    if (strncmp(arg, "--generate", strlen("--generate") + 1) == 0 ||
        strncmp(arg, "-g",         strlen("-g") + 1) == 0){
        *mode = MODE_GENERATE;
        return 1;
    }
    if (strncmp(arg, "--sort", strlen("--sort") + 1) == 0 ||
        strncmp(arg, "-s",    strlen("-s") + 1) == 0){
        *mode = MODE_SORT;
        return 1;
    }
    if (strncmp(arg, "--print", strlen("--print") + 1) == 0 ||
        strncmp(arg, "-P",     strlen("-P") + 1) == 0){
        *mode = MODE_PRINT;
        return 1;
    }
    return 0;
}

static int set_sort_type(const char* arg, sort_type_t* type){
    if (strncmp(arg, "asc", strlen("asc") + 1) == 0 ||
        strncmp(arg, "A",   strlen("A") + 1) == 0){
        *type = SORT_ASC;
        return 1;
    }
    if (strncmp(arg, "desc", strlen("desc") + 1) == 0 ||
        strncmp(arg, "D",    strlen("D") + 1) == 0){
        *type = SORT_DESC;
        return 1;
    }
    return 0;
}

static int set_sort_view(const char* arg, sort_view_t* view){
    if (strncmp(arg, "year", strlen("year") + 1) == 0 ||
        strncmp(arg, "y",    strlen("y") + 1) == 0){
        *view = SORT_YEAR;
        return 1;
    }
    if (strncmp(arg, "citations", strlen("citations") + 1) == 0 ||
        strncmp(arg, "c",         strlen("c") + 1) == 0){
        *view = SORT_CITATIONS;
        return 1;
    }
    return 0;
}

static int set_sort_method(const char* arg, sort_method_t* method){
    if (strncmp(arg, "shaker", strlen("shaker") + 1) == 0 ||
        strncmp(arg, "s",      strlen("s") + 1) == 0){
        *method = SORT_METHOD_SHAKER;
        return 1;
    }
    if (strncmp(arg, "merge", strlen("merge") + 1) == 0 ||
        strncmp(arg, "m",     strlen("m") + 1) == 0){
        *method = SORT_METHOD_MERGE;
        return 1;
    }
    return 0;
}

int analysis_args(int argq, char* argp[], program_args_t* args){
    if (args == NULL) return 0;

    args->mode = MODE_NONE;
    
    args->sort_type = SORT_ASC;
    args->sort_view = SORT_YEAR;
    args->sort_method = SORT_METHOD_SHAKER;

    args->generate_quantity = 0;
    
    args->input_file = NULL;
    args->output_file = NULL;
    args->surnames_file = NULL;
    args->words_file = NULL;
    args->initials_file = NULL;
    
    for (int i = 1; i < argq; i++){
        if (set_mode(argp[i], &args->mode)){
            continue;
        }
        if (strncmp(argp[i], "--in=", 5) == 0){
            args->input_file = argp[i] + 5;
        }
        else if (strncmp(argp[i], "-i", strlen("-i") + 1) == 0 && i + 1 < argq){
            args->input_file = argp[++i];
        }
        else if (strncmp(argp[i], "--out=", 6) == 0){
            args->output_file = argp[i] + 6;
        }
        else if (strncmp(argp[i], "-o", strlen("-o") + 1) == 0 && i + 1 < argq){
            args->output_file = argp[++i];
        }
        else if (strncmp(argp[i], "--surnames=", 11) == 0){
            args->surnames_file = argp[i] + 11;
        }
        else if (strncmp(argp[i], "-S", strlen("-S") + 1) == 0 && i + 1 < argq){
            args->surnames_file = argp[++i];
        }
        else if (strncmp(argp[i], "--words=", 8) == 0){
            args->words_file = argp[i] + 8;
        }
        else if (strncmp(argp[i], "-w", strlen("-w") + 1) == 0 && i + 1 < argq){
            args->words_file = argp[++i];
        }
        else if (strncmp(argp[i], "--initials=", 11) == 0){
            args->initials_file = argp[i] + 11;
        }
        else if (strncmp(argp[i], "-I", strlen("-I") + 1) == 0 && i + 1 < argq){
            args->initials_file = argp[++i];
        }

        else if (strncmp(argp[i], "--type=", 7) == 0){
            if (set_sort_type(argp[i] + 7, &args->sort_type) == 0){
                puts( "Ошибка: неверный --type= (нужно asc/desc)\n");
                return 0;
            }
        }
        else if (strncmp(argp[i], "-t", strlen("-t") + 1) == 0 && i + 1 < argq){
            if (set_sort_type(argp[++i], &args->sort_type) == 0){
                puts( "Ошибка: неверный -t (нужно asc/desc)\n");
                return 0;
            }
        }
        else if (strncmp(argp[i], "--view=", 7) == 0){
            if (set_sort_view(argp[i] + 7, &args->sort_view) == 0){
                puts( "Ошибка: неверный --view= (нужно year/citations)\n");
                return 0;
            }
        }
        else if (strncmp(argp[i], "-v", strlen("-v") + 1) == 0 && i + 1 < argq){
            if (set_sort_view(argp[++i], &args->sort_view) == 0){
                puts( "Ошибка: неверный -v (нужно year/citations)\n");
                return 0;
            }
        }
        else if (strncmp(argp[i], "--method=", 9) == 0){
            if (set_sort_method(argp[i] + 9, &args->sort_method) == 0){
                puts( "Ошибка: неверный --method= (нужно shaker/merge)\n");
                return 0;
            }
        }
        else if (strncmp(argp[i], "-m", strlen("-m") + 1) == 0 && i + 1 < argq){
            if (set_sort_method(argp[++i], &args->sort_method) == 0){
                puts( "Ошибка: неверный -m (нужно shaker/merge)\n");
                return 0;
            }
        }
        else if (args->mode == MODE_GENERATE && args->generate_quantity == 0){
            args->generate_quantity = atoi(argp[i]);
        }
        else {
            puts( "Ошибка\n");
            return 0;
        }
    }

    return 1;
}