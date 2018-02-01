# dynarray
A generic dynamic array implementation in C.

## Example Usage

```c
#include "dynarray.h"

int *v = dynarray_create(int);

for (int i = 0; i < 10; i++) {
    int temp = 10 - i;
    dynarray_push(v, &temp);
}

for (int i = 0; i < dynarray_length(v); i++) {
    printf("v[%d] -> %d\n", i, v[i]);
}

dynarray_destroy(v);
```
Output:
```
ints[0] -> 10
ints[1] -> 9
...
ints[8] -> 2
ints[9] -> 1
```

## Memory Layout
The array is heap-allocated and is prefixed with a three-field header containing the buffer's `capacity`, `length`, and `stride`.

* The `stride` is calculated at creation time as `sizeof(T)` where `T` is the datatype you intend to store.
* The `capacity` field stores the buffer's size.
* The `length` field keeps track of the number of elements stored in the buffer.

Macros defined in [`dynarray.h`](https://github.com/eignnx/dynarray/blob/master/dynarray.h) allow the `capacity`, `length`, and `stride` attributes to be accessed.

The dynarray is referred to _only_ by a pointer to the first element in the buffer. This allows the dynarray to be passed to most functions that were meant to operate on normal C-arrays.

![memory layout](https://github.com/eignnx/dynarray/blob/master/images/dynarray-memory-layout.png)
