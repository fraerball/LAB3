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
        int pass_swapped = bubble_iteration(rev, tmp, cmp, 1);
        if (pass_swapped != 0) {
        swapped_any = 1;
        }

        if (stack_move_all(rev, stack) == 0) {
            break;
        }
        pass_swapped = bubble_iteration(stack, tmp, cmp, 0);
        if (pass_swapped != 0) {
         swapped_any = 1;
        }

    }

    container_destroy(rev);
    container_destroy(tmp);
}
static int merge_into_one(container_t* stack, container_t* left, container_t* right, comparator_t cmp){
    if (stack == NULL || left == NULL || right == NULL || cmp == NULL){
        return 0;
    }

    container_t* temp = container_init();
    if (temp == NULL){
        return 0;
    }

    while (container_size(left) > 0 || container_size(right) > 0){
        int take_left = 0;

        if (container_size(right) == 0){
            take_left = 1;
        } else if (container_size(left) == 0){
            take_left = 0;
        } else {
            publication_t pub1;
            publication_t pub2;

            if (stack_peek(left, &pub1) == 0){
                container_destroy(temp);
                return 0;
            }

            if (stack_peek(right, &pub2) == 0){
                publication_free(&pub1);
                container_destroy(temp);
                return 0;
            }

            if (cmp(&pub1, &pub2) > 0) {
                take_left = 1;
            } else {
                take_left = 0;
            }

            publication_free(&pub1);
            publication_free(&pub2);
        }

        publication_t picked;

        if (take_left != 0){
            if (stack_pop_with_data(left, &picked) == 0){
                container_destroy(temp);
                return 0;
            }
        } else {
            if (stack_pop_with_data(right, &picked) == 0){
                container_destroy(temp);
                return 0;
            }
        }

        if (container_push(temp, &picked) == 0){
            publication_free(&picked);
            container_destroy(temp);
            return 0;
        }

        publication_free(&picked);
    }

    container_clear(stack);

    int success = stack_move_all(temp, stack);
    container_destroy(temp);
    return success;
}

static void sort_stack_merge(container_t* stack, comparator_t cmp){
    if (stack == NULL || cmp == NULL){
        return;
    }

    size_t n = container_size(stack);
    if (n < 2) {
        return;
    }

    container_t* left  = container_init();
    container_t* right = container_init();

    if (left == NULL || right == NULL){
        if (left != NULL) {
            container_destroy(left);
        }
        if (right != NULL) {
            container_destroy(right);
        }
        return;
    }

    size_t half = n / 2;

    for (size_t i = 0; i < half; i++){
        publication_t tmp_pub;

        if (stack_pop_with_data(stack, &tmp_pub) == 0){
            break;
        }

        if (container_push(left, &tmp_pub) == 0){
            publication_free(&tmp_pub);
            break;
        }

        publication_free(&tmp_pub);
    }

    while (container_size(stack) > 0){
        publication_t tmp_pub;

        if (stack_pop_with_data(stack, &tmp_pub) == 0){
            break;
        }

        if (container_push(right, &tmp_pub) == 0){
            publication_free(&tmp_pub);
            break;
        }

        publication_free(&tmp_pub);
    }

    sort_stack_merge(left, cmp);
    sort_stack_merge(right, cmp);

    merge_into_one(stack, left, right, cmp);

    container_destroy(left);
    container_destroy(right);
}
void sort(container_t* container, comparator_t cmp, sort_method_t method){
    if (container == NULL || cmp == NULL) {
        return;
    }

    if (method == SORT_METHOD_MERGE){
        sort_stack_merge(container, cmp);
    } else {
        sort_stack_shaker(container, cmp);
    }
}