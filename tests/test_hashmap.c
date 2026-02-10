#include "handrolled/hashmap.h"
#include "unity.h"
#include <stdio.h>

DEFINE_HASHMAP(IntMap, int)

void test_create_destroy(void);
void test_put_get(void);
void test_overwrite(void);
void test_contains_key(void);
void test_remove(void);
void test_copy(void);
void test_many_entries(void);

void setUp(void) {}
void tearDown(void) {}

void test_create_destroy(void) {
    IntMap map = IntMap_create(0);
    TEST_ASSERT_EQUAL_size_t(0, IntMap_length(&map));
    TEST_ASSERT_EQUAL_size_t(0, IntMap_capacity(&map));
    IntMap_destroy(&map);

    map = IntMap_create(16);
    TEST_ASSERT_EQUAL_size_t(0, IntMap_length(&map));
    TEST_ASSERT_TRUE(IntMap_capacity(&map) >= 16);
    IntMap_destroy(&map);
}

void test_put_get(void) {
    IntMap map = IntMap_create(16);

    TEST_ASSERT_TRUE(IntMap_put(&map, "alpha", 10));
    TEST_ASSERT_TRUE(IntMap_put(&map, "beta", 20));
    TEST_ASSERT_TRUE(IntMap_put(&map, "gamma", 30));
    TEST_ASSERT_EQUAL_size_t(3, IntMap_length(&map));

    int value = 0;
    TEST_ASSERT_TRUE(IntMap_get(&map, "alpha", &value));
    TEST_ASSERT_EQUAL_INT(10, value);
    TEST_ASSERT_TRUE(IntMap_get(&map, "beta", &value));
    TEST_ASSERT_EQUAL_INT(20, value);
    TEST_ASSERT_TRUE(IntMap_get(&map, "gamma", &value));
    TEST_ASSERT_EQUAL_INT(30, value);

    // nonexistent key
    TEST_ASSERT_FALSE(IntMap_get(&map, "delta", &value));

    IntMap_destroy(&map);
}

void test_overwrite(void) {
    IntMap map = IntMap_create(16);

    TEST_ASSERT_TRUE(IntMap_put(&map, "key", 100));
    TEST_ASSERT_TRUE(IntMap_put(&map, "key", 200));
    TEST_ASSERT_EQUAL_size_t(1, IntMap_length(&map));

    int value = 0;
    TEST_ASSERT_TRUE(IntMap_get(&map, "key", &value));
    TEST_ASSERT_EQUAL_INT(200, value);

    IntMap_destroy(&map);
}

void test_contains_key(void) {
    IntMap map = IntMap_create(16);

    TEST_ASSERT_TRUE(IntMap_put(&map, "exists", 42));

    TEST_ASSERT_TRUE(IntMap_contains_key(&map, "exists"));
    TEST_ASSERT_FALSE(IntMap_contains_key(&map, "missing"));

    IntMap_destroy(&map);
}

void test_remove(void) {
    IntMap map = IntMap_create(16);

    TEST_ASSERT_TRUE(IntMap_put(&map, "a", 1));
    TEST_ASSERT_TRUE(IntMap_put(&map, "b", 2));
    TEST_ASSERT_TRUE(IntMap_put(&map, "c", 3));

    int value = 0;
    TEST_ASSERT_TRUE(IntMap_remove(&map, "b", &value));
    TEST_ASSERT_EQUAL_INT(2, value);
    TEST_ASSERT_EQUAL_size_t(2, IntMap_length(&map));
    TEST_ASSERT_FALSE(IntMap_contains_key(&map, "b"));

    // remove nonexistent
    TEST_ASSERT_FALSE(IntMap_remove(&map, "z", &value));
    TEST_ASSERT_EQUAL_size_t(2, IntMap_length(&map));

    // remaining keys still accessible
    TEST_ASSERT_TRUE(IntMap_get(&map, "a", &value));
    TEST_ASSERT_EQUAL_INT(1, value);
    TEST_ASSERT_TRUE(IntMap_get(&map, "c", &value));
    TEST_ASSERT_EQUAL_INT(3, value);

    IntMap_destroy(&map);
}

void test_copy(void) {
    IntMap map = IntMap_create(16);
    TEST_ASSERT_TRUE(IntMap_put(&map, "x", 77));
    TEST_ASSERT_TRUE(IntMap_put(&map, "y", 88));

    IntMap copied = IntMap_copy(&map);
    TEST_ASSERT_EQUAL_size_t(2, IntMap_length(&copied));

    int value = 0;
    TEST_ASSERT_TRUE(IntMap_get(&copied, "x", &value));
    TEST_ASSERT_EQUAL_INT(77, value);
    TEST_ASSERT_TRUE(IntMap_get(&copied, "y", &value));
    TEST_ASSERT_EQUAL_INT(88, value);

    // modify original, copy should not be affected
    TEST_ASSERT_TRUE(IntMap_put(&map, "x", 999));
    TEST_ASSERT_TRUE(IntMap_get(&copied, "x", &value));
    TEST_ASSERT_EQUAL_INT(77, value);

    IntMap_destroy(&map);
    IntMap_destroy(&copied);
}

void test_many_entries(void) {
    IntMap map = IntMap_create(4);

    char key[16];
    for (int i = 0; i < 100; i++) {
        (void)snprintf(key, sizeof(key), "key%d", i);
        TEST_ASSERT_TRUE(IntMap_put(&map, key, i * 10));
    }
    TEST_ASSERT_EQUAL_size_t(100, IntMap_length(&map));

    int value = 0;
    for (int i = 0; i < 100; i++) {
        (void)snprintf(key, sizeof(key), "key%d", i);
        TEST_ASSERT_TRUE(IntMap_get(&map, key, &value));
        TEST_ASSERT_EQUAL_INT(i * 10, value);
    }

    IntMap_destroy(&map);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_destroy);
    RUN_TEST(test_put_get);
    RUN_TEST(test_overwrite);
    RUN_TEST(test_contains_key);
    RUN_TEST(test_remove);
    RUN_TEST(test_copy);
    RUN_TEST(test_many_entries);
    return UNITY_END();
}
