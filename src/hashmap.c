#include "handrolled/hashmap.h"
#include "handrolled/array.h"

VoidHashMap VoidHashMap_create(size_t capacity, size_t element_size) {
    // TODO
    (void)capacity;
    (void)element_size;
    return (VoidHashMap){0};
}

void VoidHashMap_destroy(VoidHashMap *self) {
    // TODO
    (void)self;
}

bool VoidHashMap_contains_key(const VoidHashMap *self, const char *key) {
    // TODO
    (void)self;
    (void)key;
    return false;
}

bool VoidHashMap_get(const VoidHashMap *self, const char *key,
                     void *out_value) {
    // TODO
    (void)self;
    (void)key;
    (void)out_value;
    return false;
}

bool VoidHashMap_put(VoidHashMap *self, const char *key, const void *value) {
    // TODO
    (void)self;
    (void)key;
    (void)value;
    return false;
}

bool VoidHashMap_remove(VoidHashMap *self, const char *key, void *out_value) {
    // TODO
    (void)self;
    (void)key;
    (void)out_value;
    return false;
}

VoidHashMap VoidHashMap_copy(const VoidHashMap *self) {
    // TODO
    (void)self;
    return (VoidHashMap){0};
}
