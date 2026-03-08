#include "handrolled/hashmap.h"
#include "handrolled/array.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LOAD_FACTOR_NUM 3
#define LOAD_FACTOR_DEN 4

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

static BucketEntry *find_entry_in_chain(BucketEntry *head, const char *key) {
    while (head) {
        if (strcmp(head->key, key) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
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

// "Fowler–Noll–Vo hash function".
// In Wikipedia, The Free Encyclopedia. Retrieved March 8, 2026, from
// https://en.wikipedia.org/w/index.php?title=Fowler%E2%80%93Noll%E2%80%93Vo_hash_function&oldid=1340921027

static size_t hash_string(const char *key) {
    const uint8_t *p = (const uint8_t *)key;

    size_t hash = 1469598103934665603ULL;

    while (*p) {
        hash ^= *p++;
        hash *= 1099511628211ULL;
    }

    return hash;
}

static size_t VoidHashMap_get_bucket_index(const VoidHashMap *self,
                                           const char *key) {
    size_t cap = VoidHashMap_capacity(self);
    if (cap == 0) {
        return 0;
    }
    return hash_string(key) % cap;
}

static BucketEntry *VoidHashMap_get_bucket_head(const VoidHashMap *self,
                                                size_t index) {
    BucketEntry *head = NULL;
    VoidArray_get(&self->buckets, (void *)&head, index);
    return head;
}

static void VoidHashMap_set_bucket_head(VoidHashMap *self, size_t index,
                                        BucketEntry *head) {
    VoidArray_set(&self->buckets, (void *)&head, index);
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
        BucketEntry *entry_head = VoidHashMap_get_bucket_head(self, i);
        destroy_bucket_chain(entry_head);
    }
    VoidArray_destroy(&self->buckets);
    self->count = 0;
    self->element_size = 0;
}

static bool VoidHashMap_resize(VoidHashMap *self, size_t new_capacity) {
    VoidArray new_buckets =
        VoidArray_create(new_capacity, sizeof(BucketEntry *));
    if (!VoidArray_is_valid(&new_buckets)) {
        return false;
    }

    size_t old_capacity = VoidHashMap_capacity(self);

    for (size_t i = 0; i < old_capacity; i++) {

        BucketEntry *entry = VoidHashMap_get_bucket_head(self, i);

        while (entry) {

            BucketEntry *next = entry->next;

            size_t index = hash_string(entry->key) % new_capacity;

            BucketEntry *head = NULL;
            VoidArray_get(&new_buckets, (void *)&head, index);

            entry->next = head;
            VoidArray_set(&new_buckets, (void *)&entry, index);

            entry = next;
        }
    }

    VoidArray_destroy(&self->buckets);
    self->buckets = new_buckets;

    return true;
}

bool VoidHashMap_put(VoidHashMap *self, const char *key, const void *value) {

    if (!self || !key || !value) {
        return false;
    }

    size_t cap = VoidHashMap_capacity(self);
    if (cap == 0) {
        return false;
    }

    size_t index = VoidHashMap_get_bucket_index(self, key);

    BucketEntry *head = VoidHashMap_get_bucket_head(self, index);

    BucketEntry *entry = find_entry_in_chain(head, key);
    if (entry) {
        memcpy(entry->value, value, self->element_size);
        return true;
    }

    BucketEntry *new_entry = BucketEntry_create(key, value, self->element_size);

    if (!new_entry) {
        return false;
    }

    new_entry->next = head;

    VoidHashMap_set_bucket_head(self, index, new_entry);

    self->count++;

    if (self->count * LOAD_FACTOR_DEN >
        VoidHashMap_capacity(self) * LOAD_FACTOR_NUM) {

        size_t new_cap = VoidHashMap_capacity(self) * 2;
        if (new_cap == 0) {
            new_cap = 8;
        }

        if (!VoidHashMap_resize(self, new_cap)) {
            return true;
        }
    }

    return true;
}

bool VoidHashMap_get(const VoidHashMap *self, const char *key,
                     void *out_value) {

    if (!self || !key) {
        return false;
    }

    size_t cap = VoidHashMap_capacity(self);
    if (cap == 0) {
        return false;
    }

    size_t index = VoidHashMap_get_bucket_index(self, key);

    BucketEntry *entry =
        find_entry_in_chain(VoidHashMap_get_bucket_head(self, index), key);

    if (!entry) {
        return false;
    }

    if (out_value) {
        memcpy(out_value, entry->value, self->element_size);
    }

    return true;
}

bool VoidHashMap_remove(VoidHashMap *self, const char *key, void *out_value) {

    if (!self || !key) {
        return false;
    }

    size_t cap = VoidHashMap_capacity(self);
    if (cap == 0) {
        return false;
    }

    size_t index = VoidHashMap_get_bucket_index(self, key);
    BucketEntry *head = VoidHashMap_get_bucket_head(self, index);

    BucketEntry *prev = NULL;
    BucketEntry *cur = head;

    while (cur) {

        if (strcmp(cur->key, key) == 0) {

            if (out_value) {
                memcpy(out_value, cur->value, self->element_size);
            }

            if (prev) {
                prev->next = cur->next;
            } else {
                BucketEntry *new_head = cur->next;
                VoidHashMap_set_bucket_head(self, index, new_head);
            }

            free(cur->key);
            free(cur->value);
            free(cur);

            self->count--;

            return true;
        }

        prev = cur;
        cur = cur->next;
    }

    return false;
}

bool VoidHashMap_contains_key(const VoidHashMap *self, const char *key) {
    return VoidHashMap_get(self, key, NULL);
}

VoidHashMap VoidHashMap_copy(const VoidHashMap *self) {

    if (!self) {
        return (VoidHashMap){0};
    }

    size_t cap = VoidHashMap_capacity(self);

    VoidHashMap new_map = VoidHashMap_create(cap, self->element_size);

    if (!VoidArray_is_valid(&new_map.buckets)) {
        return (VoidHashMap){0};
    }

    for (size_t i = 0; i < cap; i++) {

        BucketEntry *entry = VoidHashMap_get_bucket_head(self, i);

        while (entry) {

            VoidHashMap_put(&new_map, entry->key, entry->value);

            entry = entry->next;
        }
    }

    return new_map;
}
