#include "dynarray.h"

/*

Dynamic Array

A dynarray has three hidden fields of type `size_t` stored in it's header:
    - capacity: size in `stride`-sized units of the allocated buffer.
    - length: the number of `stride`-sized units currently filled.
    - stride: the sizeof the datatype being stored in the dynarray.

To get the ith element in the array, you can use bracket notation (`arr[i]`),
or the `dynarray_get` method which does bounds checking.

To set the ith element of the array, use either bracket notation
(`arr[i] = x;`), or the `dynarray_set` method which does bounds checking.
*/

// Returns a pointer to the start of a new dynarray (after the header) which
// has `init_cap` units of `stride` bytes.
void *_dynarray_create(size_t init_cap, size_t stride)
{
    size_t header_size = DYNARRAY_FIELDS * sizeof(size_t);
    size_t arr_size = init_cap * stride;
    size_t *arr = (size_t *) malloc(header_size + arr_size);
    arr[CAPACITY] = init_cap;
    arr[LENGTH] = 0;
    arr[STRIDE] = stride;
    return (void *) (arr + DYNARRAY_FIELDS);
}

// Returns the dynarray's field which is specified by passing
// one of CAPACITY, LENGTH, STRIDE.
size_t _dynarray_field_get(void *arr, size_t field)
{
    return ((size_t *)(arr) - DYNARRAY_FIELDS)[field];
}

void _dynarray_push(void *arr, void *xptr)
{
    memcpy(arr + dynarray_length(arr) * dynarray_stride(arr), xptr, dynarray_stride(arr));
}

// Removes the last element in the array, but copys it to `*dest` first.
void _dynarray_pop(void *arr, void *dest)
{
    //memcpy();
}


