#include "handrolled/hashmap.h"
#include "handrolled/array.h"
#include <stdlib.h>
#include <string.h>

typedef struct BucketEntry {
    char *key;
    void *value;
    struct BucketEntry *next;
} BucketEntry;

static BucketEntry *BucketEntry_create(const char *key, const void *value,
                                       size_t value_size) {
    BucketEntry *be = malloc(sizeof(*be));
    if (!be) {
        return NULL;
    }

    // copy string of *key of input to bucket
    size_t len = strlen(key);
    char *key_copy = malloc(len + 1);
    if (!key_copy) {
        free(be);
        return NULL;
    }
    strcpy(key_copy, key);
    be->key = key_copy;

    be->value = malloc(value_size);
    if (!be->value) {
        free(be->key);
        free(be);
        return NULL;
    }
    memcpy(be->value, value, value_size);
    be->next = NULL;

    return be;
}

static void destroy_bucket_chain(BucketEntry *be_head) {
    BucketEntry *current_be = be_head;
    while (current_be) {
        BucketEntry *next = current_be->next;
        free(current_be->key);
        free(current_be->value);
        free(current_be);

        current_be = next;
    }
}

VoidHashMap VoidHashMap_create(size_t capacity, size_t element_size) {

    if (element_size == 0) {
        return (VoidHashMap){0};
    }
    if (capacity == 0) {
        return (VoidHashMap){
            .buckets = {0},
            .count = 0,
            .element_size = element_size,
        };
    }

    VoidArray buckets = VoidArray_create(capacity, sizeof(BucketEntry *));
    if (!VoidArray_is_valid(&buckets)) {
        return (VoidHashMap){0};
    }

    return (VoidHashMap){
        .buckets = buckets,
        .count = 0,
        .element_size = element_size,
    };
}

void VoidHashMap_destroy(VoidHashMap *self) {

    if (!self) {
        return;
    }

    size_t capacity = VoidHashMap_capacity(self);
    for (size_t i = 0; i < capacity; ++i) {
        BucketEntry *entry_head = NULL;
        VoidArray_get(&self->buckets, (void *)&entry_head, i);
        destroy_bucket_chain(entry_head);
    }
    VoidArray_destroy(&self->buckets);
    self->count = 0;
    self->element_size = 0;
}

bool VoidHashMap_contains_key(const VoidHashMap *self, const char *key) {
    // TODO:
    (void)self;
    (void)key;
    return false;
}

bool VoidHashMap_get(const VoidHashMap *self, const char *key,
                     void *out_value) {
    // TODO:
    (void)self;
    (void)key;
    (void)out_value;
    return false;
}

bool VoidHashMap_put(VoidHashMap *self, const char *key, const void *value) {
    // TODO:
    (void)self;
    (void)key;
    (void)value;
    (void)BucketEntry_create;

    return false;
}

bool VoidHashMap_remove(VoidHashMap *self, const char *key, void *out_value) {
    // TODO:
    (void)self;
    (void)key;
    (void)out_value;
    return false;
}

VoidHashMap VoidHashMap_copy(const VoidHashMap *self) {
    // TODO:
    (void)self;
    return (VoidHashMap){0};
}
