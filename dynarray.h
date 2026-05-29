#ifndef DYNARRAY
#define DYNARRAY

#include <stdlib.h>     // malloc
#include <string.h>     // memcpy

/**
 * @mainpage Dynamic Array Library: dynarray
 *
 * @section overview Overview
 * A generic dynamic array implementation in C.
 * A dynarray of type T can be passed to any function that operates on vanilla T arrays.
 * Inspiration taken from this article by solarianprogrammer.
 *
 * @section features Features
 * - Automatic resizing
 * - Generic storage using void pointers
 * - Bounds-checked access (optional API)
 *
 * @section usage Basic Usage
 * @code
 * #include "dynarray.h"
 *
 * int *v = dynarray_create(int);
 *
 * for (int i = 0; i < 10; i++) {
 *     dynarray_push(v, i);
 *     dynarray_push_rval(v, 100 - i);
 * }
 *
 * for (int i = 0; i < dynarray_length(v); i++) {
 *     printf("v[%d] -> %d\n", i, v[i]);
 * }
 *
 * dynarray_destroy(v);
 * @endcode
 * Output:
 * @code
 * v[0] -> 0
 * v[1] -> 100
 * v[2] -> 1
 * v[3] -> 99
 * ...
 * v[18] -> 9
 * v[19] -> 91
 * @endcode
 * @section Memory Layout
 * The array is heap-allocated and is prefixed with a three-field header containing the buffer's capacity, length, and stride.
 *
 * - The stride is calculated at creation time as sizeof(T) where T is the datatype you intend to store.
 * - The capacity field stores the buffer's size.
 * - The length field keeps track of the number of elements stored in the buffer.
 *
 * Macros defined in dynarray.h allow the capacity, length, and stride attributes to be accessed.
 *
 * A dynarray is referred to only by a pointer to the first element in its buffer.
 * This allows a dynarray to be passed to any function that operates on regular C-arrays.
 */

/* Structure of a dynarray:
 * size_t capacity
 * size_t length
 * size_t stride
 * void *memory
 */

enum {
    CAPACITY,
    LENGTH,
    STRIDE,
    DYNARRAY_FIELDS
};

void *_dynarray_create(size_t length, size_t stride);
void _dynarray_destroy(void *arr);

size_t _dynarray_field_get(void *arr, size_t field);
void _dynarray_field_set(void *arr, size_t field, size_t value);

void *_dynarray_resize(void *arr);

void *_dynarray_push(void *arr, void *xptr);
void _dynarray_pop(void *arr, void *dest);

#define DYNARRAY_DEFAULT_CAP 1
#define DYNARRAY_RESIZE_FACTOR 2

/**
 * @brief 
 *
 * @param type
 */
#define dynarray_create(type) _dynarray_create(DYNARRAY_DEFAULT_CAP, sizeof(type))

#define dynarray_create_prealloc(type, capacity) _dynarray_create(capacity, sizeof(type))
#define dynarray_destroy(arr) _dynarray_destroy(arr)

#define dynarray_push(arr, x) arr = _dynarray_push(arr, &x)
#define dynarray_push_rval(arr, x) \
    do { \
        __auto_type temp = x; \
        arr = _dynarray_push(arr, &temp); \
    } while (0)

#define dynarray_pop(arr, xptr) _dynarray_pop(arr, xptr)

#define dynarray_capacity(arr) _dynarray_field_get(arr, CAPACITY)
#define dynarray_length(arr) _dynarray_field_get(arr, LENGTH)
#define dynarray_stride(arr) _dynarray_field_get(arr, STRIDE)

#endif // DYNARRAY

