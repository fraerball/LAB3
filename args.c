#include <stdlib.h>
#include "args.h"

void args_init(program_args_t* args) {
    if (args == NULL) {
        return;
    }

    args->mode = MODE_NONE;

    args->input_file = NULL;
    args->output_file = NULL;

    args->generate_count = 0;

    args->sort_field = SORT_FIELD_NONE;
    args->sort_order = SORT_ORDER_NONE;
}

void args_free(program_args_t* args) {
    if (args == NULL) {
        return;
    }

    free(args->input_file);
    free(args->output_file);

    args->input_file = NULL;
    args->output_file = NULL;
}
