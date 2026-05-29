#ifndef DYNARRAY
#define DYNARRAY

#include <stdlib.h>     // malloc
#include <string.h>     // memcpy

/* Structure of a dynarray:
 * size_t capacity
 * size_t length
 * size_t stride
 * void *memory
 */

enum dynarray_field_offset {
    DYNARRAY_CAPACITY_FIELD,
    DYNARRAY_LENGTH_FIELD,
    DYNARRAY_STRIDE_FIELD,
    DYNARRAY_FIELDS
};

void *_dynarray_create(size_t length, size_t stride);
void _dynarray_destroy(void *arr);

size_t _dynarray_field_get(void *arr, size_t field);
void _dynarray_field_set(void *arr, size_t field, size_t value);

void *_dynarray_resize(void *arr);

void *_dynarray_push(void *arr, void *xptr);
void _dynarray_pop(void *arr, void *dest);
void _dynarray_swap_remove(void *arr, size_t index, void *dest);

#define DYNARRAY_DEFAULT_CAP 1
#define DYNARRAY_RESIZE_FACTOR 2

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

/**
 * @brief Removes an element from the vector and returns it.
 *
 * The removed element is replaced by the last element of the vector.
 * This does not preserve ordering of the remaining elements
 *
 * @param arr Pointer to the dynamic array.
 * @param idx Element in the array.
 * @param xptr Pointer to memory where the removed elements will be stored.
 *
 * @note The caller must ensure @p xptr points to a valid memory location
 *       large enough to hold one element of the array's element type.
 */
#define dynarray_swap_remove(arr, idx, xptr) _dynarray_swap_remove(arr, idx, xptr)

#define dynarray_capacity(arr) _dynarray_field_get(arr, DYNARRAY_CAPACITY_FIELD)
#define dynarray_length(arr) _dynarray_field_get(arr, DYNARRAY_LENGTH_FIELD)
#define dynarray_stride(arr) _dynarray_field_get(arr, DYNARRAY_STRIDE_FIELD)

#endif // DYNARRAY

