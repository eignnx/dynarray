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
// If reallocation fails the function will return NULL.
void *_dynarray_create(size_t init_cap, size_t stride)
{
    size_t header_size = DYNARRAY_FIELDS * sizeof(size_t);
    size_t arr_size = init_cap * stride;
    size_t *arr = (size_t *) malloc(header_size + arr_size);
    if (arr == NULL) return NULL; // propagate failure
    arr[DYNARRAY_CAPACITY_FIELD] = init_cap;
    arr[DYNARRAY_LENGTH_FIELD] = 0;
    arr[DYNARRAY_STRIDE_FIELD] = stride;
    return (void *) (arr + DYNARRAY_FIELDS);
}

// Allocates a new dynarray with twice the size of the one passed in, and retaining
// the values that the original stored.
// If reallocation fails the function will return NULL.
void *_dynarray_resize(void *arr)
{
    size_t new_capacity = DYNARRAY_RESIZE_FACTOR * dynarray_capacity(arr);
    size_t stride = dynarray_stride(arr);
    size_t header_size = DYNARRAY_FIELDS * sizeof(size_t);
    size_t arr_size = new_capacity * stride;
    void *temp = realloc((arr) - header_size, header_size + arr_size);
    if (temp == NULL) return NULL; // propagate failure
    temp += header_size;
    size_t *field_capacity = _dynarray_field_at(temp, DYNARRAY_CAPACITY_FIELD);
    *field_capacity = new_capacity; 
    return temp;
}

// Places a new value at the next available space.
// If array is full then it will reallocate with a bigger size.
// If reallocation fails the function will return NULL;
void *_dynarray_push(void *arr, void *xptr)
{
    if(arr == NULL || xptr == NULL) return NULL;

    if (dynarray_length(arr) >= dynarray_capacity(arr)) {
        arr = _dynarray_resize(arr);
        if (arr == NULL) return NULL;
    }

    memcpy(arr + dynarray_length(arr) * dynarray_stride(arr), xptr, dynarray_stride(arr));
    size_t *length_field = _dynarray_field_at(arr, DYNARRAY_LENGTH_FIELD);
    *length_field += 1;
    return arr;
}

// Removes the last element in the array, but copies it to `*dest` first.
void _dynarray_pop(void *arr, void *dest)
{
    if(arr == NULL || dest == NULL) return;

    memcpy(dest, arr + (dynarray_length(arr) - 1) * dynarray_stride(arr), dynarray_stride(arr));
    size_t *length_field = _dynarray_field_at(arr, DYNARRAY_LENGTH_FIELD); // Decrement length.
    *length_field -= 1;
}


