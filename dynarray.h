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

#define DYNARRAY_DEFAULT_CAP 1
#define DYNARRAY_RESIZE_FACTOR 2

/**
 * @brief Creates a new dynamic array.
 *
 * Allocates and initializes a new dynamic array with an initial capacity of DYNARRAY_DEFAULT_CAP elements,
 * each the size of the element type.
 * The returned pointer refers to the start of the usable array memory
 *
 * @param type Type of elements in array.
 * @return Pointer to the newly created dynamic array, or NULL if allocation fails.
 */
#define dynarray_create(type) _dynarray_create(DYNARRAY_DEFAULT_CAP, sizeof(type))

/**
 * @brief Creates a new dynamic array with custom size.
 *
 * Allocates and initializes a new dynamic array with an initial capacity of @p capacity elements,
 * each the size of the element type.
 * The returned pointer refers to the start of the usable array memory
 *
 * @param type Type of elements in array.
 * @param capacity Initial number of elements the array can hold.
 * @return Pointer to the newly created dynamic array, or NULL if allocation fails.
 */
#define dynarray_create_prealloc(type, capacity) _dynarray_create(capacity, sizeof(type))

/**
 * @brief Frees a dynamic array.
 *
 * Deallocates the memory block associated with the dynamic array.
 *
 * @param arr Pointer to the dynamic array to be destroyed.
 *
 * @note This function does not modify the caller's pointer; it is the caller's
 *       responsibility to set it to NULL if needed.
 */
#define dynarray_destroy(arr) _dynarray_destroy(arr)

/**
 * @brief Appends a new element to the dynamic array.
 *
 * Inserts the value pointed to by @p x into the next available position
 * in the dynamic array. If the array is full, it will be reallocated with
 * increased capacity before insertion.
 *
 * @param arr Pointer to the dynamic array.
 * @param x Pointer to the value to be inserted.
 * @return Pointer to the (possibly reallocated) dynamic array, or NULL if
 *         reallocation fails.
 *
 * @note If reallocation fails, the original array remains unchanged.
 */
#define dynarray_push(arr, x) arr = _dynarray_push(arr, &x)

/**
 * @brief Appends a value expression to the dynamic array.
 *
 * Evaluates @p x and inserts a copy of that value into the next available position in the dynamic
 * array. If the array is full, it will be reallocated with increased capacity before insertion.
 *
 * Unlike ::dynarray_push, this macro accepts rvalues and arbitrary
 * expressions, including literals, function calls, and computed values.
 *
 * @param arr Pointer to the dynamic array.
 * @param x Value or expression to be inserted.
 *
 * @note The expression @p x is evaluated exactly once.
 *
 * @note Array expressions passed as @p x undergo array-to-pointer decay,
 *       which may produce different behavior compared to ::dynarray_push.
 *
 * @note If reallocation fails, the original array remains unchanged.
 */
#define dynarray_push_rval(arr, x) \
    do { \
        __auto_type temp = x; \
        arr = _dynarray_push(arr, &temp); \
    } while (0)

/**
 * @brief Removes the last element from the dynamic array.
 *
 * Pops the last element in the dynamic array and copies its value into
 * the memory pointed by @p xptr before removal.
 *
 * @param arr Pointer to the dynamic array.
 * @param xptr Pointer to memory where the removed elements will be stored.
 *
 * @note The caller must ensure @p xptr points to a valid memory location
 *       large enough to hold one element of the array's element type.
 */
#define dynarray_pop(arr, xptr) _dynarray_pop(arr, xptr)

#define dynarray_capacity(arr) _dynarray_field_get(arr, DYNARRAY_CAPACITY_FIELD)
#define dynarray_length(arr) _dynarray_field_get(arr, DYNARRAY_LENGTH_FIELD)
#define dynarray_stride(arr) _dynarray_field_get(arr, DYNARRAY_STRIDE_FIELD)

#endif // DYNARRAY

