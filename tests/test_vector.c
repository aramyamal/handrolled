#include "handrolled/vector.h"
#include <assert.h>
#include <stdio.h>

DEFINE_VECTOR(IntVector, int)

static void test_create_destroy(void) {
    printf("Test: Create and destroy\n");

    IntVector vec = IntVector_create(0);
    assert(IntVector_length(&vec) == 0);
    assert(IntVector_capacity(&vec) == 0);
    IntVector_destroy(&vec);
    assert(vec._internal.array.data == NULL);
    assert(vec._internal.length == 0);
    assert(vec._internal.array.length == 0);

    // test with initial capacity
    vec = IntVector_create(5);
    assert(IntVector_length(&vec) == 0);
    assert(IntVector_capacity(&vec) >= 5);
    IntVector_destroy(&vec);
    assert(vec._internal.array.data == NULL);
    assert(vec._internal.length == 0);
    assert(vec._internal.array.length == 0);
}

static void test_push_elements(void) {
    printf("Test: Push elements\n");

    IntVector vec = IntVector_create(2);
    assert(IntVector_push(&vec, 10) == true);
    assert(IntVector_push(&vec, 20) == true);
    assert(IntVector_push(&vec, 30) == true);
    assert(IntVector_length(&vec) == 3);
    assert(IntVector_capacity(&vec) >= 3);
    IntVector_destroy(&vec);
}

static void test_get_set_elements(void) {
    printf("Test: Get and set elements\n");

    IntVector vec = IntVector_create(5);
    IntVector_push(&vec, 10);
    IntVector_push(&vec, 20);
    IntVector_push(&vec, 30);

    int value = 0;
    // get
    assert(IntVector_get(&vec, &value, 0) == true);
    assert(value == 10);
    assert(IntVector_get(&vec, &value, 1) == true);
    assert(value == 20);
    assert(IntVector_get(&vec, &value, 2) == true);
    assert(value == 30);

    // set
    assert(IntVector_set(&vec, 25, 1) == true);
    assert(IntVector_get(&vec, &value, 1) == true);
    assert(value == 25);

    // out of bounds
    assert(IntVector_get(&vec, &value, 3) == false);
    assert(IntVector_set(&vec, 40, 3) == false);

    IntVector_destroy(&vec);
}

static void test_pop_elements(void) {
    printf("Test: Pop elements\n");

    IntVector vec = IntVector_create(5);
    IntVector_push(&vec, 10);
    IntVector_push(&vec, 20);
    IntVector_push(&vec, 30);

    int value = 0;
    assert(IntVector_pop(&vec, &value) == true);
    assert(value == 30);
    assert(IntVector_length(&vec) == 2);

    assert(IntVector_pop(&vec, &value) == true);
    assert(value == 20);
    assert(IntVector_length(&vec) == 1);

    assert(IntVector_pop(&vec, &value) == true);
    assert(value == 10);
    assert(IntVector_length(&vec) == 0);

    // pop from empty vector
    assert(IntVector_pop(&vec, &value) == false);

    IntVector_destroy(&vec);
}

static void test_clear_vector(void) {
    printf("Test: Clear vector\n");

    IntVector vec = IntVector_create(2);
    IntVector_push(&vec, 10);
    IntVector_push(&vec, 20);
    IntVector_push(&vec, 30);

    assert(IntVector_length(&vec) == 3);
    size_t capacity = IntVector_capacity(&vec);
    IntVector_clear(&vec);
    assert(IntVector_length(&vec) == 0);

    // capacity should remain
    assert(IntVector_capacity(&vec) == capacity);

    // verify we can still add elements after clear
    assert(IntVector_push(&vec, 40) == true);
    assert(IntVector_length(&vec) == 1);
    int value = 0;
    assert(IntVector_get(&vec, &value, 0) == true);
    assert(value == 40);

    IntVector_destroy(&vec);
}

static void test_copy_vector(void) {
    printf("Test: Copy vector\n");

    IntVector vec = IntVector_create(5);
    IntVector_push(&vec, 100);
    IntVector_push(&vec, 200);

    IntVector copied = IntVector_copy(&vec);
    assert(IntVector_length(&copied) == 2);

    int value = 0;
    assert(IntVector_get(&copied, &value, 0) == true);
    assert(value == 100);
    assert(IntVector_get(&copied, &value, 1) == true);
    assert(value == 200);

    // modify original, copy should not be affected
    IntVector_set(&vec, 300, 0);
    assert(IntVector_get(&copied, &value, 0) == true);
    assert(value == 100); // copy unchanged

    IntVector_destroy(&vec);
    IntVector_destroy(&copied);
}

int main(void) {
    test_create_destroy();
    test_push_elements();
    test_get_set_elements();
    test_pop_elements();
    test_clear_vector();
    test_copy_vector();

    return 0;
}
