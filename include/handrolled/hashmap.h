#ifndef HANDROLLED_HASHMAP_H
#define HANDROLLED_HASHMAP_H

#include "array.h"
#include "common.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    VoidArray buckets;
    // capacity: buckets.length
    size_t count;
    size_t element_size;
} VoidHashMap;

static inline size_t VoidHashMap_capacity(const VoidHashMap *self) {
    return self ? VoidArray_length(&self->buckets) : 0;
}

static inline size_t VoidHashMap_length(const VoidHashMap *self) {
    return self ? self->count : 0;
}

VoidHashMap VoidHashMap_create(size_t capacity, size_t element_size);
void VoidHashMap_destroy(VoidHashMap *self);
bool VoidHashMap_contains_key(const VoidHashMap *self, const char *key);
bool VoidHashMap_get(const VoidHashMap *self, const char *key, void *out_value);
bool VoidHashMap_put(VoidHashMap *self, const char *key, const void *value);
bool VoidHashMap_remove(VoidHashMap *self, const char *key, void *out_value);
VoidHashMap VoidHashMap_copy(const VoidHashMap *self);

#define DEFINE_HASHMAP(name, T)                                                \
    typedef struct {                                                           \
        VoidHashMap _internal;                                                 \
    } name;                                                                    \
                                                                               \
    static inline HR_MAYBE_UNUSED name name##_create(size_t capacity) {        \
        return (name){._internal = VoidHashMap_create(capacity, sizeof(T))};   \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED void name##_destroy(name *self) {            \
        VoidHashMap_destroy(&self->_internal);                                 \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED size_t name##_length(const name *self) {     \
        return VoidHashMap_length(&self->_internal);                           \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED size_t name##_capacity(const name *self) {   \
        return VoidHashMap_capacity(&self->_internal);                         \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED bool name##_contains_key(const name *self,   \
                                                           const char *key) {  \
        return VoidHashMap_contains_key(&self->_internal, key);                \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED bool name##_get(                             \
        const name *self, const char *key, T *out_value) {                     \
        return VoidHashMap_get(&self->_internal, key, out_value);              \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED bool name##_put(name *self, const char *key, \
                                                  const T value) {             \
        return VoidHashMap_put(&self->_internal, key, &value);                 \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED bool name##_remove(                          \
        name *self, const char *key, T *out_value) {                           \
        return VoidHashMap_remove(&self->_internal, key, out_value);           \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED name name##_copy(const name *self) {         \
        return (name){._internal = VoidHashMap_copy(&self->_internal)};        \
    }

#endif // HANDROLLED_HASHMAP_H
