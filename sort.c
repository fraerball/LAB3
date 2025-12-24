#include "sort.h"
#include "publication.h"
#include "container.h"
#include <stdlib.h>

void sort(container_t* container, comparator_t cmp) {
    if (!container || !cmp) return;
    
    size_t n = container_size(container);
    if (n < 2) return;
    
    size_t left = 0;
    size_t right = n - 1;
    int swapped = 1;
    
    while (left < right && swapped) {
        swapped = 0;
        
        for (size_t i = left; i < right; i++) {
            publication_t publication1, publication2;
            if (container_get(container, i, &publication1) && container_get(container, i + 1, &publication2)) {
                
                if (cmp(&publication1, &publication2) > 0) {
                    container_swap(container, i, i + 1);
                    swapped = 1;
                }
                publication_free(&publication1);
                publication_free(&publication2);
            }
        }
        
        if (!swapped) break;
        right--;
        swapped = 0;
        
        for (size_t i = right; i > left; i--) {
            publication_t publication1, publication2;
            if (container_get(container, i - 1, &publication1) && container_get(container, i, &publication2)) {
                if (cmp(&publication1, &publication2) > 0) {
                    container_swap(container, i - 1, i);
                    swapped = 1;
                }
                publication_free(&publication1);
                publication_free(&publication2);
            }
        }
        
        left++;
    }
}