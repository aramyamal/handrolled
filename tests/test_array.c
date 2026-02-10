#include "handrolled/array.h"
#include "unity.h"
#include <assert.h>

DEFINE_ARRAY(IntArray, int)

void test_create_destroy(void);
void test_get_set_elements(void);
void test_copy_array(void);
void test_resize_array(void);

void setUp(void) {}
void tearDown(void) {}

void test_create_destroy(void) {
    IntArray arr = IntArray_create(0);
    TEST_ASSERT_EQUAL_size_t(0, IntArray_length(&arr));
    IntArray_destroy(&arr);
    TEST_ASSERT_NULL(arr._internal.data);
    TEST_ASSERT_EQUAL_size_t(0, arr._internal.length);

    // test with initial length
    arr = IntArray_create(5);
    TEST_ASSERT_EQUAL_size_t(5, IntArray_length(&arr));
    IntArray_destroy(&arr);
    TEST_ASSERT_NULL(arr._internal.data);
    TEST_ASSERT_EQUAL_size_t(0, arr._internal.length);
}

void test_get_set_elements(void) {
    IntArray arr = IntArray_create(5);
    TEST_ASSERT_EQUAL_size_t(5, IntArray_length(&arr));

    // set
    TEST_ASSERT_TRUE(IntArray_set(&arr, 42, 0));
    TEST_ASSERT_TRUE(IntArray_set(&arr, 100, 4));

    // get
    int value = 0;
    TEST_ASSERT_TRUE(IntArray_get(&arr, &value, 0));
    TEST_ASSERT_EQUAL_INT(42, value);
    TEST_ASSERT_TRUE(IntArray_get(&arr, &value, 4));
    TEST_ASSERT_EQUAL_INT(100, value);

    // out of bounds
    TEST_ASSERT_FALSE(IntArray_get(&arr, &value, 5));
    TEST_ASSERT_FALSE(IntArray_set(&arr, 999, 5));

    IntArray_destroy(&arr);
}

void test_copy_array(void) {
    IntArray arr = IntArray_create(5);
    TEST_ASSERT_TRUE(IntArray_set(&arr, 42, 0));
    TEST_ASSERT_TRUE(IntArray_set(&arr, 100, 4));

    IntArray copy = IntArray_copy(&arr);
    TEST_ASSERT_EQUAL_size_t(IntArray_length(&arr), IntArray_length(&copy));

    int value = 0;
    TEST_ASSERT_TRUE(IntArray_get(&copy, &value, 0));
    TEST_ASSERT_EQUAL_INT(42, value);
    TEST_ASSERT_TRUE(IntArray_get(&copy, &value, 4));
    TEST_ASSERT_EQUAL_INT(100, value);

    // modify original, copy should not be affected
    TEST_ASSERT_TRUE(IntArray_set(&arr, 777, 0));
    TEST_ASSERT_TRUE(IntArray_get(&copy, &value, 0));
    TEST_ASSERT_EQUAL_INT(42, value);

    IntArray_destroy(&arr);
    IntArray_destroy(&copy);
}

void test_resize_array(void) {
    IntArray arr = IntArray_create(5);
    TEST_ASSERT_TRUE(IntArray_set(&arr, 42, 0));
    TEST_ASSERT_TRUE(IntArray_set(&arr, 100, 4));

    // shrink
    TEST_ASSERT_TRUE(IntArray_resize(&arr, 3));
    TEST_ASSERT_EQUAL_size_t(3, IntArray_length(&arr));

    int value = -1;
    TEST_ASSERT_TRUE(IntArray_get(&arr, &value, 0));
    TEST_ASSERT_EQUAL_INT(42, value);
    TEST_ASSERT_FALSE(IntArray_get(&arr, &value, 4));

    // grow (new slots should be zero-initialized if your impl guarantees
    // that)
    TEST_ASSERT_TRUE(IntArray_resize(&arr, 8));
    TEST_ASSERT_EQUAL_size_t(8, IntArray_length(&arr));

    value = 1234;
    TEST_ASSERT_TRUE(IntArray_get(&arr, &value, 7));
    TEST_ASSERT_EQUAL_INT(0, value);

    IntArray_destroy(&arr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_destroy);
    RUN_TEST(test_get_set_elements);
    RUN_TEST(test_copy_array);
    RUN_TEST(test_resize_array);
    return UNITY_END();
}
