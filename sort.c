#include "sort.h"
#include "container.h"
#include "publication.h"
#include "container.h"
#include <stdlib.h>

void sort(container_t* container, comparator_t cmp) {
    if (!container || !cmp) return;

    size_t n = container_size(container);
    if (n < 2) return;

    size_t left = 0;
    size_t right = n - 1;
    int swapped;

    while (left < right) {
        swapped = 0;
        for (size_t i = left; i < right; i++) {
            publication_t pub1, pub2;
            if (!container_get(container, i, &pub1)) return;
            if (!container_get(container, i + 1, &pub2)) { publication_free(&pub1); return; }

            if (cmp(&pub1, &pub2) > 0) {
                container_update(container, i, &pub2);
                container_update(container, i + 1, &pub1);
                swapped = 1;
            }

            publication_free(&pub1);
            publication_free(&pub2);
        }

        if (!swapped) break;
        right--;

        swapped = 0;
        for (size_t i = right; i > left; i--) {
            publication_t pub1, pub2;
            if (!container_get(container, i - 1, &pub1)) return;
            if (!container_get(container, i, &pub2)) { publication_free(&pub1); return; }

            if (cmp(&pub1, &pub2) > 0) {
                container_update(container, i - 1, &pub2);
                container_update(container, i, &pub1);
                swapped = 1;
            }

            publication_free(&pub1);
            publication_free(&pub2);
        }

        left++;
    }
}