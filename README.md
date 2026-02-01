# handrolled

Handrolled, type-safe and bounds-checked data structure implementations for C.

## Data Structures

### Array
Fixed-size array with type safety:
```c
#include "handrolled/array.h"
#include <stdio.h>

DEFINE_ARRAY(Int32Array, int32_t)

int main(void) {
    Int32Array test_array = Int32Array_create(5);
    int32_t j = 5;
    for (size_t i = 0; i < Int32Array_length(&test_array); ++i) {
        Int32Array_set(&test_array, j++, i);
    }

    printf("array is: \n");

    for (size_t i = 0; i < Int32Array_length(&test_array); ++i) {
        int32_t out = 0;
        Int32Array_get(&test_array, &out, i);
        printf("%d\n", out);
    }

    return 0;
}
```

### Vector
Dynamic array with automatic resizing:
```c
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
```

## API Reference
See the header files in `include/handrolled/` for complete API documentation:
- `include/handrolled/array.h` - Fixed-size arrays
- `include/handrolled/vector.h` - Dynamic arrays

## Status
**Work in Progress**: This is a hobby project for getting better understanding of data structues and C. More will be added as I explore. Check `include/handrolled/` for the actual API since this README might not be up-to-date.
