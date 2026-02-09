#include "handrolled/hashmap.h"
#include <assert.h>
#include <stdio.h>

DEFINE_HASHMAP(IntMap, int)

static void test_create_destroy(void) {
    printf("Test: Create and destroy\n");

    IntMap map = IntMap_create(0);
    assert(IntMap_length(&map) == 0);
    assert(IntMap_capacity(&map) == 0);
    IntMap_destroy(&map);

    map = IntMap_create(16);
    assert(IntMap_length(&map) == 0);
    assert(IntMap_capacity(&map) >= 16);
    IntMap_destroy(&map);
}

static void test_put_get(void) {
    printf("Test: Put and get\n");

    IntMap map = IntMap_create(16);

    assert(IntMap_put(&map, "alpha", 10) == true);
    assert(IntMap_put(&map, "beta", 20) == true);
    assert(IntMap_put(&map, "gamma", 30) == true);
    assert(IntMap_length(&map) == 3);

    int value = 0;
    assert(IntMap_get(&map, "alpha", &value) == true);
    assert(value == 10);
    assert(IntMap_get(&map, "beta", &value) == true);
    assert(value == 20);
    assert(IntMap_get(&map, "gamma", &value) == true);
    assert(value == 30);

    // nonexistent key
    assert(IntMap_get(&map, "delta", &value) == false);

    IntMap_destroy(&map);

    (void)value;
}

static void test_overwrite(void) {
    printf("Test: Overwrite existing key\n");

    IntMap map = IntMap_create(16);

    assert(IntMap_put(&map, "key", 100) == true);
    assert(IntMap_put(&map, "key", 200) == true);
    assert(IntMap_length(&map) == 1);

    int value = 0;
    assert(IntMap_get(&map, "key", &value) == true);
    assert(value == 200);

    IntMap_destroy(&map);

    (void)value;
}

static void test_contains_key(void) {
    printf("Test: Contains key\n");

    IntMap map = IntMap_create(16);

    IntMap_put(&map, "exists", 42);

    assert(IntMap_contains_key(&map, "exists") == true);
    assert(IntMap_contains_key(&map, "missing") == false);

    IntMap_destroy(&map);
}

static void test_remove(void) {
    printf("Test: Remove\n");

    IntMap map = IntMap_create(16);

    IntMap_put(&map, "a", 1);
    IntMap_put(&map, "b", 2);
    IntMap_put(&map, "c", 3);

    int value = 0;
    assert(IntMap_remove(&map, "b", &value) == true);
    assert(value == 2);
    assert(IntMap_length(&map) == 2);
    assert(IntMap_contains_key(&map, "b") == false);

    // remove nonexistent
    assert(IntMap_remove(&map, "z", &value) == false);
    assert(IntMap_length(&map) == 2);

    // remaining keys still accessible
    assert(IntMap_get(&map, "a", &value) == true);
    assert(value == 1);
    assert(IntMap_get(&map, "c", &value) == true);
    assert(value == 3);

    IntMap_destroy(&map);

    (void)value;
}

static void test_copy(void) {
    printf("Test: Copy\n");

    IntMap map = IntMap_create(16);
    IntMap_put(&map, "x", 77);
    IntMap_put(&map, "y", 88);

    IntMap copied = IntMap_copy(&map);
    assert(IntMap_length(&copied) == 2);

    int value = 0;
    assert(IntMap_get(&copied, "x", &value) == true);
    assert(value == 77);
    assert(IntMap_get(&copied, "y", &value) == true);
    assert(value == 88);

    // modify original, copy should not be affected
    IntMap_put(&map, "x", 999);
    assert(IntMap_get(&copied, "x", &value) == true);
    assert(value == 77);

    IntMap_destroy(&map);
    IntMap_destroy(&copied);

    (void)value;
}

static void test_many_entries(void) {
    printf("Test: Many entries (trigger resize)\n");

    IntMap map = IntMap_create(4);

    char key[16];
    for (int i = 0; i < 100; i++) {
        (void)snprintf(key, sizeof(key), "key%d", i);
        assert(IntMap_put(&map, key, i * 10) == true);
    }
    assert(IntMap_length(&map) == 100);

    int value = 0;
    for (int i = 0; i < 100; i++) {
        (void)snprintf(key, sizeof(key), "key%d", i);
        assert(IntMap_get(&map, key, &value) == true);
        assert(value == i * 10);
    }

    IntMap_destroy(&map);

    (void)value;
}

int main(void) {
    test_create_destroy();
    test_put_get();
    test_overwrite();
    test_contains_key();
    test_remove();
    test_copy();
    test_many_entries();

    return 0;
}
