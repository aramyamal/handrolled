#ifndef HANDROLLED_VECTOR_H
#define HANDROLLED_VECTOR_H

#include "common.h"
#include "array.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    VoidArray array;
    size_t length;
} VoidVector;

static inline size_t VoidVector_length(const VoidVector *self) {
    return self ? self->length : 0;
}

static inline size_t VoidVector_capacity(const VoidVector *self) {
    return self ? VoidArray_length(&self->array) : 0;
}

VoidVector VoidVector_create(size_t capacity, size_t element_size);
void VoidVector_destroy(VoidVector *self);
bool VoidVector_push(VoidVector *self, const void *value);
bool VoidVector_get(const VoidVector *self, void *out_value, size_t index);
bool VoidVector_set(VoidVector *self, const void *value, size_t index);
bool VoidVector_pop(VoidVector *self, void *out_value);
void VoidVector_clear(VoidVector *self);
VoidVector VoidVector_copy(const VoidVector *self);

#define DEFINE_VECTOR(name, T)                                                 \
    typedef struct {                                                           \
        VoidVector _internal;                                                  \
    } name;                                                                    \
                                                                               \
    static inline HR_MAYBE_UNUSED name name##_create(size_t capacity) {        \
        return (name){._internal = VoidVector_create(capacity, sizeof(T))};    \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED void name##_destroy(name *self) {            \
        VoidVector_destroy(&self->_internal);                                  \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED size_t name##_length(const name *self) {     \
        return VoidVector_length(&self->_internal);                            \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED size_t name##_capacity(const name *self) {   \
        return VoidVector_capacity(&self->_internal);                          \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED bool name##_push(name *self,                 \
                                                   const T value) {            \
        return VoidVector_push(&self->_internal, &value);                      \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED bool name##_get(                             \
        const name *self, T *out_value, size_t index) {                        \
        return VoidVector_get(&self->_internal, out_value, index);             \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED bool name##_set(name *self, const T value,   \
                                                  size_t index) {              \
        return VoidVector_set(&self->_internal, &value, index);                \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED bool name##_pop(name *self, T *out_value) {  \
        return VoidVector_pop(&self->_internal, out_value);                    \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED void name##_clear(name *self) {              \
        VoidVector_clear(&self->_internal);                                    \
    }                                                                          \
                                                                               \
    static inline HR_MAYBE_UNUSED name name##_copy(const name *self) {         \
        return (name){._internal = VoidVector_copy(&self->_internal)};         \
    }

#endif // HANDROLLED_VECTOR_H
