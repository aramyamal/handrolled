#ifndef HANDROLLED_ARRAY_H
#define HANDROLLED_ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    void *data;
    size_t length;
    size_t element_size;
} VoidArray;

static inline bool VoidArray_is_valid(const VoidArray *self) {
    return self && self->data && self->length > 0 && self->element_size > 0;
}

static inline bool VoidArray_is_valid_index(const VoidArray *self,
                                            const size_t index) {
    return VoidArray_is_valid(self) && index < self->length;
}

static inline size_t VoidArray_length(const VoidArray *self) {
    return self ? self->length : 0;
}

VoidArray VoidArray_create(size_t length, size_t element_size);
void VoidArray_destroy(VoidArray *self);
bool VoidArray_get(const VoidArray *self, void *out_value, size_t index);
bool VoidArray_set(VoidArray *self, const void *value, size_t index);
VoidArray VoidArray_copy(const VoidArray *self);
bool VoidArray_resize(VoidArray *self, size_t new_length);

#define DEFINE_ARRAY(name, T)                                                  \
    typedef struct {                                                           \
        VoidArray _internal;                                                   \
    } name;                                                                    \
                                                                               \
    static inline name name##_create(size_t length) {                          \
        return (name){._internal = VoidArray_create(length, sizeof(T))};       \
    }                                                                          \
                                                                               \
    static inline void name##_destroy(name *self) {                            \
        VoidArray_destroy(&self->_internal);                                   \
    }                                                                          \
                                                                               \
    static inline size_t name##_length(const name *self) {                     \
        return VoidArray_length(&self->_internal);                             \
    }                                                                          \
                                                                               \
    static inline bool name##_get(const name *self, T *out_value,              \
                                  size_t index) {                              \
        return VoidArray_get(&self->_internal, out_value, index);              \
    }                                                                          \
                                                                               \
    static inline bool name##_set(name *self, const T value, size_t index) {   \
        return VoidArray_set(&self->_internal, &value, index);                 \
    }                                                                          \
                                                                               \
    static inline name name##_copy(const name *self) {                         \
        return (name){._internal = VoidArray_copy(&self->_internal)};          \
    }                                                                          \
                                                                               \
    static inline bool name##_resize(name *self, size_t new_length) {          \
        return VoidArray_resize(&self->_internal, new_length);                 \
    }

#endif // HANDROLLED_ARRAY_H
