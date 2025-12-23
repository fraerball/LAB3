#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"

static int set_mode(const char* arg, mode_t* mode) {
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
