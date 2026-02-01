#include "handrolled/vector.h"
#include <stdio.h>

DEFINE_VECTOR(StringVector, char *)
int main(void) {

    StringVector filenames = StringVector_create(2); // capacity 2
    printf("Initial capacity: %zu\n", StringVector_capacity(&filenames));

    StringVector_push(&filenames, ".txt");
    StringVector_push(&filenames, ".log");
    StringVector_push(&filenames, ".md");
    printf("Capacity after pushing: %zu\n", StringVector_capacity(&filenames));

    // print contents
    for (size_t i = 0; i < StringVector_length(&filenames); i++) {
        char *extension = NULL;
        StringVector_get(&filenames, &extension, i);
        printf("Extension: %s\n", extension);
    }

    StringVector_destroy(&filenames);
}
