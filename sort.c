#include "sort.h"
#include "container.h"
#include "publication.h"
#include "container.h"
#include <stdlib.h>

static int stack_peek(const container_t* stack, publication_t* ptr_pub){
    if (stack == NULL || ptr_pub == NULL) {
        return 0;
    }

    size_t n = container_size(stack);
    if (n == 0) {
        return 0;
    }

    return container_get(stack, n - 1, ptr_pub);
}

static int stack_pop_with_data(container_t* stack, publication_t* ptr_pub){
    if (stack_peek(stack, ptr_pub) == 0) {
        return 0;
    }

    if (container_pop(stack) == 0){
        publication_free(ptr_pub);
        return 0;
    }

    return 1;
}

static int stack_move_all(container_t* src, container_t* dst){
    if (src == NULL || dst == NULL) {
        return 0;
    }

    while (container_size(src) > 0){
        publication_t tmp;

        if (stack_pop_with_data(src, &tmp) == 0) {
            return 0;
        }

        if (container_push(dst, &tmp) == 0){
            publication_free(&tmp);
            return 0;
        }

        publication_free(&tmp);
    }

    return 1;
}

static int bubble_iteration(container_t* stack, container_t* tmp, comparator_t cmp, int forward){
    if (stack == NULL || tmp == NULL || cmp == NULL) {
        return 0;
    }

    size_t n = container_size(stack);
    if (n < 2) {
        return 0;
    }

    int swapped = 0;

    publication_t prev;
    if (stack_pop_with_data(stack, &prev) == 0) {
        return 0;
    }

    for (size_t i = 1; i < n; i++){
        publication_t cur;
        if (stack_pop_with_data(stack, &cur) == 0){
            publication_free(&prev);
            return 0;
        }

        int rel = cmp(&prev, &cur);

        int need_swap;
        if (forward != 0) {
            need_swap = (rel > 0);
        } else {
            need_swap = (rel < 0);
        }

        if (need_swap != 0){
            if (container_push(tmp, &cur) == 0){
                publication_free(&cur);
                publication_free(&prev);
                return 0;
            }
            publication_free(&cur);
            swapped = 1;
        } else {
            if (container_push(tmp, &prev) == 0){
                publication_free(&cur);
                publication_free(&prev);
                return 0;
            }
            publication_free(&prev);
            prev = cur;
        }
    }

    if (container_push(tmp, &prev) == 0){
        publication_free(&prev);
        return 0;
    }
    publication_free(&prev);

    if (stack_move_all(tmp, stack) == 0) {
        return 0;
    }

    return swapped;
}

static void sort_stack_shaker(container_t* stack, comparator_t cmp){
    if (stack == NULL || cmp == NULL) {
        return;
    }

    if (container_size(stack) < 2) {
        return;
    }

    container_t* rev = container_init();
    container_t* tmp = container_init();

    if (rev == NULL || tmp == NULL){
        if (rev != NULL) {
            container_destroy(rev);
        }
        if (tmp != NULL) {
            container_destroy(tmp);
        }
        return;
    }

    int swapped_any = 1;

    while (swapped_any != 0){
        swapped_any = 0;

        if (stack_move_all(stack, rev) == 0) {
            break;
        }
        int pass_swapped = bubble_pass(rev, tmp, cmp, 1);
        if (pass_swapped != 0) {
        swapped_any = 1;
        }

        if (stack_move_all(rev, stack) == 0) {
            break;
        }
        pass_swapped = bubble_pass(stack, tmp, cmp, 0);
        if (pass_swapped != 0) {
         swapped_any = 1;
        }

    }

    container_destroy(rev);
    container_destroy(tmp);
}