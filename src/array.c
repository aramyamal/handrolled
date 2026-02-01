#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "handrolled/array.h"

static inline VoidArray VoidArray_null(void) {
    return (VoidArray){.data = NULL, .length = 0, .element_size = 0};
}

VoidArray VoidArray_create(const size_t length, const size_t element_size) {
    if (length == 0 || element_size == 0) {
        return VoidArray_null();
    }

    void *data = calloc(length, element_size);
    if (!data) {
        return VoidArray_null();
    }

    return (VoidArray){
        .data = data,
        .length = length,
        .element_size = element_size,
    };
}

void VoidArray_destroy(VoidArray *self) {
    free(self->data);
    self->data = NULL;
    self->element_size = 0;
    self->length = 0;
}

bool VoidArray_get(const VoidArray *self, void *out_value, const size_t index) {
    if (!VoidArray_is_valid_index(self, index) || !out_value) {
        return false;
    }

    // convert to 1 byte type to do generic pointer arithmetic with element_size
    uint8_t *element = (uint8_t *)self->data + (self->element_size * index);
    memcpy(out_value, element, self->element_size);
    return true;
}

bool VoidArray_set(VoidArray *self, const void *value, const size_t index) {
    if (!VoidArray_is_valid_index(self, index) || !value) {
        return false;
    }

    uint8_t *element = (uint8_t *)self->data + (self->element_size * index);
    memcpy(element, value, self->element_size);
    return true;
}

VoidArray VoidArray_copy(const VoidArray *self) {
    if (!VoidArray_is_valid(self)) {
        return VoidArray_null();
    }
    size_t bytes = self->length * self->element_size;
    void *data_copy = malloc(bytes);
    if (!data_copy) {
        return VoidArray_null();
    }
    memcpy(data_copy, self->data, bytes);
    return (VoidArray){
        .data = data_copy,
        .length = self->length,
        .element_size = self->element_size,
    };
}

bool VoidArray_resize(VoidArray *self, size_t new_length) {
    if (!VoidArray_is_valid(self)) {
        return false;
    }

    if (new_length == 0) {
        VoidArray_destroy(self);
        return true;
    }

    if (new_length == self->length) {
        return true;
    }

    void *new_data = realloc(self->data, new_length * self->element_size);
    if (!new_data) {
        return false;
    }

    if (new_length > self->length) {
        size_t extra_bytes = (new_length - self->length) * self->element_size;
        void *first_new_element =
            (uint8_t *)new_data + (self->length * self->element_size);
        memset(first_new_element, 0, extra_bytes);
    }

    self->data = new_data;
    self->length = new_length;

    return true;
}
