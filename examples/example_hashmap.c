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
