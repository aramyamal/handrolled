#include "handrolled/array.h"
#include <assert.h>
#include <stdio.h>

DEFINE_ARRAY(IntArray, int)

static void test_create_destroy(void) {
    printf("Test: Create and destroy\n");

    IntArray arr = IntArray_create(0);
    assert(IntArray_length(&arr) == 0);
    IntArray_destroy(&arr);
    assert(arr._internal.data == NULL);
    assert(arr._internal.length == 0);

    // test with initial length
    arr = IntArray_create(5);
    assert(IntArray_length(&arr) == 5);
    IntArray_destroy(&arr);
    assert(arr._internal.data == NULL);
    assert(arr._internal.length == 0);
}

static void test_get_set_elements(void) {
    printf("Test: Get and set elements\n");

    IntArray arr = IntArray_create(5);
    assert(IntArray_length(&arr) == 5);

    // set
    assert(IntArray_set(&arr, 42, 0) == true);
    assert(IntArray_set(&arr, 100, 4) == true);

    // get
    int value = 0;
    assert(IntArray_get(&arr, &value, 0) == true);
    assert(value == 42);
    assert(IntArray_get(&arr, &value, 4) == true);
    assert(value == 100);

    // out of bounds
    assert(IntArray_get(&arr, &value, 5) == false);
    assert(IntArray_set(&arr, 999, 5) == false);

    IntArray_destroy(&arr);

    (void)value;
}

static void test_copy_array(void) {
    printf("Test: Copy\n");

    IntArray arr = IntArray_create(5);
    IntArray_set(&arr, 42, 0);
    IntArray_set(&arr, 100, 4);

    IntArray copy = IntArray_copy(&arr);
    assert(IntArray_length(&copy) == IntArray_length(&arr));

    int value = 0;
    assert(IntArray_get(&copy, &value, 0) == true);
    assert(value == 42);
    assert(IntArray_get(&copy, &value, 4) == true);
    assert(value == 100);

    // modify original, copy should not be affected
    assert(IntArray_set(&arr, 777, 0) == true);
    assert(IntArray_get(&copy, &value, 0) == true);
    assert(value == 42);

    IntArray_destroy(&arr);
    IntArray_destroy(&copy);

    (void)value;
}

static void test_resize_array(void) {
    printf("Test: Resize\n");

    IntArray arr = IntArray_create(5);
    assert(IntArray_set(&arr, 42, 0) == true);
    assert(IntArray_set(&arr, 100, 4) == true);

    // shrink
    assert(IntArray_resize(&arr, 3) == true);
    assert(IntArray_length(&arr) == 3);

    int value = -1;
    assert(IntArray_get(&arr, &value, 0) == true);
    assert(value == 42);
    assert(IntArray_get(&arr, &value, 4) == false);

    // grow (new slots should be zero-initialized if your impl guarantees that)
    assert(IntArray_resize(&arr, 8) == true);
    assert(IntArray_length(&arr) == 8);

    value = 1234;
    assert(IntArray_get(&arr, &value, 7) == true);
    assert(value == 0);

    IntArray_destroy(&arr);

    (void)value;
}

int main(void) {
    test_create_destroy();
    test_get_set_elements();
    test_copy_array();
    test_resize_array();
    return 0;
}
