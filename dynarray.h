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
void *_dynarray_resize(void *arr);

void *_dynarray_push(void *arr, void *xptr);
void _dynarray_pop(void *arr, void *dest);

#define DYNARRAY_DEFAULT_CAP 1
#define DYNARRAY_RESIZE_FACTOR 2

#define dynarray_create(type) _dynarray_create(DYNARRAY_DEFAULT_CAP, sizeof(type))
#define dynarray_create_prealloc(type, capacity) _dynarray_create(capacity, sizeof(type))
#define dynarray_destroy(arr) \
    do { \
        if((arr) == NULL) break; \
        free((void *)(arr) - DYNARRAY_FIELDS * sizeof(size_t)); \
        (arr) = NULL; \
    } while(0)

#define dynarray_push(arr, x) arr = _dynarray_push(arr, &x)
#define dynarray_push_rval(arr, x) \
    do { \
        __auto_type temp = x; \
        (arr) = _dynarray_push(arr, &temp); \
    } while (0)

#define dynarray_pop(arr, xptr) _dynarray_pop(arr, xptr)

// Returns the dynarray's field which is specified by passing
// one of CAPACITY, LENGTH, STRIDE. (getter/setter)
#define _dynarray_field_at(arr, field_value) ((arr) == NULL || (field_value) >= DYNARRAY_FIELDS ? \
                                            NULL: \
                                            ((size_t *)(arr) - DYNARRAY_FIELDS) + (field_value))

#define dynarray_capacity(arr) *_dynarray_field_at(arr, DYNARRAY_CAPACITY_FIELD)
#define dynarray_length(arr) *_dynarray_field_at(arr, DYNARRAY_LENGTH_FIELD)
#define dynarray_stride(arr) *_dynarray_field_at(arr, DYNARRAY_STRIDE_FIELD)

#endif // DYNARRAY

