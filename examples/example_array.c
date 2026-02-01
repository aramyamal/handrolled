#include "handrolled/array.h"
#include <stdio.h>

DEFINE_ARRAY(Int32Array, int32_t)

int main(void) {
    Int32Array test_array = Int32Array_create(5);
    int32_t j = 5;
    for (size_t i = 0; i < Int32Array_length(&test_array); ++i) {
        Int32Array_set(&test_array, j++, i);
    }

    printf("array is: \n");

    for (size_t i = 0; i < Int32Array_length(&test_array); ++i) {
        int32_t out = 0;
        Int32Array_get(&test_array, &out, i);
        printf("%d\n", out);
    }

    return 0;
}
