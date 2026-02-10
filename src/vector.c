#include "handrolled/vector.h"
#include "handrolled/array.h"
#include <stdbool.h>
#include <stddef.h>

static inline VoidVector VoidVector_null(void) { return (VoidVector){0}; }

static inline bool VoidVector_is_valid(const VoidVector *self) {
    return self && VoidArray_is_valid(&self->array);
}

static inline bool VoidVector_is_valid_index(const VoidVector *self,
                                             size_t index) {
    return self && index < self->length &&
           VoidArray_is_valid_index(&self->array, index);
}

VoidVector VoidVector_create(size_t capacity, size_t element_size) {
    if (capacity == 0) {
        capacity = 1; // clamp to 1
    }
    VoidArray array = VoidArray_create(capacity, element_size);
    if (!VoidArray_is_valid(&array)) {
        return VoidVector_null();
    }
    return (VoidVector){
        .array = array,
        .length = 0,
    };
}

void VoidVector_destroy(VoidVector *self) {
    VoidArray_destroy(&self->array);
    self->length = 0;
}

bool VoidVector_push(VoidVector *self, const void *value) {
    if (!value || !VoidVector_is_valid(self)) {
        return false;
    }
    size_t length = self->length;
    size_t capacity = self->array.length;
    if (self->length >= capacity) {
        size_t new_capacity = capacity * 2;

        if (new_capacity / 2 != capacity) {
            return false; // overflow
        }
        if (!VoidArray_resize(&self->array, new_capacity)) {
            return false; // failed to resize
        }
    }
    if (!VoidArray_set(&self->array, value, length)) {
        return false;
    };
    self->length++;
    return true;
}

bool VoidVector_get(const VoidVector *self, void *out_value, size_t index) {
    return VoidVector_is_valid_index(self, index)
               ? VoidArray_get(&self->array, out_value, index)
               : false;
}

bool VoidVector_set(VoidVector *self, const void *value, size_t index) {
    return VoidVector_is_valid_index(self, index)
               ? VoidArray_set(&self->array, value, index)
               : false;
}

bool VoidVector_pop(VoidVector *self, void *out_value) {
    size_t length = VoidVector_length(self);
    if (!out_value || length == 0 || !VoidVector_is_valid(self)) {
        return false;
    }
    if (!VoidArray_get(&self->array, out_value, length - 1)) {
        return false;
    }
    self->length--;
    return true;
}

void VoidVector_clear(VoidVector *self) {
    if (VoidVector_is_valid(self)) {
        self->length = 0;
    }
}

VoidVector VoidVector_copy(const VoidVector *self) {
    if (!VoidVector_is_valid(self)) {
        return VoidVector_null();
    }
    return (VoidVector){
        .array = VoidArray_copy(&self->array),
        .length = self->length,
    };
}
