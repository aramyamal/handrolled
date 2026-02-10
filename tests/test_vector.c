#include "handrolled/vector.h"
#include "unity.h"

DEFINE_VECTOR(IntVector, int)

void test_create_destroy(void);
void test_push_elements(void);
void test_get_set_elements(void);
void test_pop_elements(void);
void test_clear_vector(void);
void test_copy_vector(void);

void setUp(void) {}
void tearDown(void) {}

void test_create_destroy(void) {
    IntVector vec = IntVector_create(0);
    TEST_ASSERT_EQUAL_size_t(0, IntVector_length(&vec));
    TEST_ASSERT_EQUAL_size_t(0, IntVector_capacity(&vec));
    IntVector_destroy(&vec);
    TEST_ASSERT_NULL(vec._internal.array.data);
    TEST_ASSERT_EQUAL_size_t(0, vec._internal.length);
    TEST_ASSERT_EQUAL_size_t(0, vec._internal.array.length);

    // test with initial capacity
    vec = IntVector_create(5);
    TEST_ASSERT_EQUAL_size_t(0, IntVector_length(&vec));
    TEST_ASSERT_TRUE(IntVector_capacity(&vec) >= 5);
    IntVector_destroy(&vec);
    TEST_ASSERT_NULL(vec._internal.array.data);
    TEST_ASSERT_EQUAL_size_t(0, vec._internal.length);
    TEST_ASSERT_EQUAL_size_t(0, vec._internal.array.length);
}

void test_push_elements(void) {
    IntVector vec = IntVector_create(2);
    TEST_ASSERT_TRUE(IntVector_push(&vec, 10));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 20));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 30));
    TEST_ASSERT_EQUAL_size_t(3, IntVector_length(&vec));
    TEST_ASSERT_TRUE(IntVector_capacity(&vec) >= 3);
    IntVector_destroy(&vec);
}

void test_get_set_elements(void) {
    IntVector vec = IntVector_create(5);
    TEST_ASSERT_TRUE(IntVector_push(&vec, 10));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 20));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 30));

    int value = 0;
    // get
    TEST_ASSERT_TRUE(IntVector_get(&vec, &value, 0));
    TEST_ASSERT_EQUAL_INT(10, value);
    TEST_ASSERT_TRUE(IntVector_get(&vec, &value, 1));
    TEST_ASSERT_EQUAL_INT(20, value);
    TEST_ASSERT_TRUE(IntVector_get(&vec, &value, 2));
    TEST_ASSERT_EQUAL_INT(30, value);

    // set
    TEST_ASSERT_TRUE(IntVector_set(&vec, 25, 1));
    TEST_ASSERT_TRUE(IntVector_get(&vec, &value, 1));
    TEST_ASSERT_EQUAL_INT(25, value);

    // out of bounds
    TEST_ASSERT_FALSE(IntVector_get(&vec, &value, 3));
    TEST_ASSERT_FALSE(IntVector_set(&vec, 40, 3));

    IntVector_destroy(&vec);
}

void test_pop_elements(void) {
    IntVector vec = IntVector_create(5);
    TEST_ASSERT_TRUE(IntVector_push(&vec, 10));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 20));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 30));

    int value = 0;
    TEST_ASSERT_TRUE(IntVector_pop(&vec, &value));
    TEST_ASSERT_EQUAL_INT(30, value);
    TEST_ASSERT_EQUAL_size_t(2, IntVector_length(&vec));

    TEST_ASSERT_TRUE(IntVector_pop(&vec, &value));
    TEST_ASSERT_EQUAL_INT(20, value);
    TEST_ASSERT_EQUAL_size_t(1, IntVector_length(&vec));

    TEST_ASSERT_TRUE(IntVector_pop(&vec, &value));
    TEST_ASSERT_EQUAL_INT(10, value);
    TEST_ASSERT_EQUAL_size_t(0, IntVector_length(&vec));

    // pop from empty vector
    TEST_ASSERT_FALSE(IntVector_pop(&vec, &value));

    IntVector_destroy(&vec);
}

void test_clear_vector(void) {
    IntVector vec = IntVector_create(2);
    TEST_ASSERT_TRUE(IntVector_push(&vec, 10));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 20));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 30));

    TEST_ASSERT_EQUAL_size_t(3, IntVector_length(&vec));
    size_t capacity = IntVector_capacity(&vec);
    IntVector_clear(&vec);
    TEST_ASSERT_EQUAL_size_t(0, IntVector_length(&vec));

    // capacity should remain
    TEST_ASSERT_EQUAL_size_t(capacity, IntVector_capacity(&vec));

    // verify we can still add elements after clear
    TEST_ASSERT_TRUE(IntVector_push(&vec, 40));
    TEST_ASSERT_EQUAL_size_t(1, IntVector_length(&vec));
    int value = 0;
    TEST_ASSERT_TRUE(IntVector_get(&vec, &value, 0));
    TEST_ASSERT_EQUAL_INT(40, value);

    IntVector_destroy(&vec);
}

void test_copy_vector(void) {
    IntVector vec = IntVector_create(5);
    TEST_ASSERT_TRUE(IntVector_push(&vec, 100));
    TEST_ASSERT_TRUE(IntVector_push(&vec, 200));

    IntVector copied = IntVector_copy(&vec);
    TEST_ASSERT_EQUAL_size_t(2, IntVector_length(&copied));

    int value = 0;
    TEST_ASSERT_TRUE(IntVector_get(&copied, &value, 0));
    TEST_ASSERT_EQUAL_INT(100, value);
    TEST_ASSERT_TRUE(IntVector_get(&copied, &value, 1));
    TEST_ASSERT_EQUAL_INT(200, value);

    // modify original, copy should not be affected
    TEST_ASSERT_TRUE(IntVector_set(&vec, 300, 0));
    TEST_ASSERT_TRUE(IntVector_get(&copied, &value, 0));
    TEST_ASSERT_EQUAL_INT(100, value); // copy unchanged

    IntVector_destroy(&vec);
    IntVector_destroy(&copied);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_destroy);
    RUN_TEST(test_push_elements);
    RUN_TEST(test_get_set_elements);
    RUN_TEST(test_pop_elements);
    RUN_TEST(test_clear_vector);
    RUN_TEST(test_copy_vector);
    return UNITY_END();
}
