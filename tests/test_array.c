#include "handrolled/array.h"
#include <assert.h>

DEFINE_ARRAY(IntArray, int)

int main(void) {

    // create array
    IntArray arr = IntArray_create(5);
    assert(IntArray_length(&arr) == 5);

    // set and get values
    assert(IntArray_set(&arr, 42, 0) == true);
    assert(IntArray_set(&arr, 100, 4) == true);

    int value = 0;
    assert(IntArray_get(&arr, &value, 0) == true);
    assert(value == 42);
    assert(IntArray_get(&arr, &value, 4) == true);
    assert(value == 100);

    // test bounds checking
    assert(IntArray_get(&arr, &value, 5) == false);
    assert(IntArray_set(&arr, 999, 5) == false);

    // test copy
    IntArray arr_copy = IntArray_copy(&arr);
    assert(IntArray_length(&arr_copy) == IntArray_length(&arr));
    assert(IntArray_get(&arr_copy, &value, 0) == true);
    assert(value == 42);
    assert(IntArray_get(&arr_copy, &value, 4) == true);
    assert(value == 100);

    // test resize
    assert(IntArray_resize(&arr, 3) == true);
    assert(IntArray_length(&arr) == 3);
    assert(IntArray_get(&arr, &value, 0) == true);
    assert(value == 42);
    assert(IntArray_get(&arr, &value, 4) == false);

    assert(IntArray_resize(&arr, 8) == true);
    assert(IntArray_length(&arr) == 8);
    assert(IntArray_get(&arr, &value, 7) == true); // check zero initialization
    assert(value == 0);

    // cleanup
    IntArray_destroy(&arr);
    assert(IntArray_length(&arr) == 0);

    return 0;
}
