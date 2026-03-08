# handrolled

Hand rolled, type-safe and bounds-checked data structure implementations for C. This is a hobby project for getting better understanding of data structures and C.

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

### HashMap

Simple hash table implementation with c-strings as keys.

```c
#include "handrolled/hashmap.h"
#include <stdio.h>

DEFINE_HASHMAP(IntMap, int)

int main(void) {

    IntMap ages = IntMap_create(4);

    // insert values
    IntMap_put(&ages, "Alice", (int){25});
    IntMap_put(&ages, "Bob", (int){30});
    IntMap_put(&ages, "Charlie", (int){28});

    printf("Map contains %zu entries\n", IntMap_length(&ages));

    // lookup values
    int age = 0;
    if (IntMap_get(&ages, "Bob", &age)) {
        printf("Bob is %d years old\n", age);
    }

    if (IntMap_contains_key(&ages, "Alice")) {
        printf("Alice exists in the map\n");
    }

    // remove a value
    if (IntMap_remove(&ages, "Charlie", &age)) {
        printf("Removed Charlie (age %d)\n", age);
    }

    printf("Map now contains %zu entries\n", IntMap_length(&ages));

    IntMap_destroy(&ages);

    return 0;
}
```

## API Reference

See the header files in `include/handrolled/` for complete API documentation:

- `include/handrolled/array.h` - Fixed-size arrays
- `include/handrolled/vector.h` - Dynamic arrays
- `include/handrolled/hashmap.h` - Simple hash table with c-strings as keys
