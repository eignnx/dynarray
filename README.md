# dynarray
A generic dynamic array implementation in C.

## Example Usage

```c
#include "dynarray.h"

int *ints = dynarray_create(int);

for (int i = 0; i < 10; i++)
    dynarray_push(ints, &i);

for (int i = 0; i < dynarray_length(ints); i++)
    printf("ints[%d] -> %d\n", ints[i]);
```
Output:
```
ints[0] -> 0
ints[1] -> 1
...
ints[8] -> 8
ints[9] -> 9
```
