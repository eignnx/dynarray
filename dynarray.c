#include "dynarray.h"

/**
 * @file dynarray.h
 * @brief Dynamic array implementation with hidden metadata header.
 *
 * A dynamic array (dynarray) stores three internal metadata fields in a hidden header:
 *
 * - @b capacity: Number of elements (of size `stride`) allocated in the buffer.
 * - @b length: Number of elements currently stored in the array.
 * - @b stride: Size (in bytes) of each element stored in the array.
 *
 * The user-facing pointer returned by dynarray functions points to the first
 * usable element, not the internal header.
 *
 * ## Accessing elements
 *
 * Elements can be accessed either via bracket notation:
 * @code
 * arr[i]
 * @endcode
 *
 * or via safe accessor functions (e.g. `dynarray_get`) which perform bounds checking.
 *
 * Elements can be assigned using:
 * @code
 * arr[i] = value;
 * @endcode
 *
 * or via `dynarray_set`, which also performs bounds checking.
 *
 * ## Design notes
 *
 * The header is stored immediately before the returned pointer in memory,
 * allowing O(1) access to metadata fields while keeping the API pointer clean.
 */

/**
 * @defgroup dynarray_internal Internal Implementation
 * @brief Low-level helpers (not for user use).
 */

/**
 * @ingroup dynarray_internal
 * @brief Creates a new dynamic array and its associated metadata.
 *
 * Allocates and initializes a new dynamic array with an initial capacity
 * of @p init_cap elements, each of size @p stride bytes. The returned
 * pointer refers to the start of the usable array memory (after the internal
 * metadata header).
 *
 * @param init_cap Initial number of elements the array can hold.
 * @param stride Size in bytes of the elements in the array.
 * @return Pointer to the newly created dynamic array, or NULL if allocation fails.
 *
 * @note The returned pointer does not point to the internal header; it points
 *       directly to the array storage region.
 */
void *_dynarray_create(size_t init_cap, size_t stride)
{
    size_t header_size = DYNARRAY_FIELDS * sizeof(size_t);
    size_t arr_size = init_cap * stride;
    size_t *arr = (size_t *) malloc(header_size + arr_size);
    if (arr == NULL) return NULL; // propagate failure
    arr[CAPACITY] = init_cap;
    arr[LENGTH] = 0;
    arr[STRIDE] = stride;
    return (void *) (arr + DYNARRAY_FIELDS);
}

/**
 * @ingroup dynarray_internal
 * @brief Frees a dynamic array and its associated metadata.
 *
 * Deallocates the memory block associated with the dynamic array,
 * including both the user-facing array data and the hidden metadata
 * stored before the array pointer.
 *
 * @param arr Pointer to the dynamic array to be destroyed.
 *
 * @note This function does not modify the caller's pointer; it is the caller's
 *       responsibility to set it to NULL if needed.
 */
void _dynarray_destroy(void *arr)
{
    free(arr - DYNARRAY_FIELDS * sizeof(size_t));
}

/**
 * @ingroup dynarray_internal
 * @brief Retrieves a metadata field from a dynamic array.
 *
 * Returns a specific internal field of the dynamic array, selected via
 * the `field` parameter.
 *
 * Valid fields include:
 * - CAPACITY
 * - LENGTH
 * - STRIDE
 *
 * @param arr Pointer to the dynamic array.
 * @param field Identifier specifying which field to retrieve (CAPACITY, LENGTH, or STRIDE).
 * @return Value of the requested field as a size_t.
 */
size_t _dynarray_field_get(void *arr, size_t field)
{
    return ((size_t *)(arr) - DYNARRAY_FIELDS)[field];
}

/**
 * @ingroup dynarray_internal
 * @brief Sets a metadata field from a dynamic array.
 *
 * Sets a specific internal field of the dynamic array, selected via
 * the `field` parameter.
 *
 * Valid fields include:
 * - CAPACITY
 * - LENGTH
 * - STRIDE
 *
 * @param arr Pointer to the dynamic array.
 * @param field Identifier specifying which field to retrieve (CAPACITY, LENGTH, or STRIDE).
 * @param value The value to set.
 */
void _dynarray_field_set(void *arr, size_t field, size_t value)
{
    ((size_t *)(arr) - DYNARRAY_FIELDS)[field] = value;
}


/**
 * @ingroup dynarray_internal
 * @brief Resizes a dynamic array to double its current capacity.
 *
 * Allocates a new dynamic array with twice the size of the input array,
 * copying over all values from the original array.
 *
 * @param arr Pointer to the original dynamic array.
 * @return Pointer to the newly allocated array, or NULL if reallocation fails.
 *
 * @note On failure, the original array remains unchanged.
 */
void *_dynarray_resize(void *arr)
{
    void *temp = _dynarray_create( // Allocate new dynarray w/ more space.
        DYNARRAY_RESIZE_FACTOR * dynarray_capacity(arr),
        dynarray_stride(arr)
    );
    if (temp == NULL) return NULL; // propagate failure
    memcpy(temp, arr, dynarray_length(arr) * dynarray_stride(arr)); // Copy erythin' over.
    _dynarray_field_set(temp, LENGTH, dynarray_length(arr)); // Set `length` field.
    _dynarray_destroy(arr); // Free previous array.
    return temp;
}

/**
 * @ingroup dynarray_internal
 * @brief Appends a new element to the dynamic array.
 *
 * Inserts the value pointed to by @p xptr into the next available position
 * in the dynamic array. If the array is full, it will be reallocated with
 * increased capacity before insertion.
 *
 * @param arr Pointer to the dynamic array.
 * @param xptr Pointer to the value to be inserted.
 * @return Pointer to the (possibly reallocated) dynamic array, or NULL if
 *         reallocation fails.
 *
 * @note If reallocation fails, the original array remains unchanged.
 */
void *_dynarray_push(void *arr, void *xptr)
{
    if (dynarray_length(arr) >= dynarray_capacity(arr)) {
        arr = _dynarray_resize(arr);
        if (arr == NULL) return NULL;
    }

    memcpy(arr + dynarray_length(arr) * dynarray_stride(arr), xptr, dynarray_stride(arr));
    _dynarray_field_set(arr, LENGTH, dynarray_length(arr) + 1);
    return arr;
}

/**
 * @ingroup dynarray_internal
 * @brief Removes the last element from the dynamic array.
 *
 * Pops the last element in the dynamic array and copies its value into
 * the memory pointed by @p dest before removal.
 *
 * @param arr Pointer to the dynamic array.
 * @param dest Pointer to memory where the removed elements will be stored.
 *
 * @note The caller must ensure @p dest points to a valid memory location
 *       large enough to hold one element of the array's element type.
 */
void _dynarray_pop(void *arr, void *dest)
{
    memcpy(dest, arr + (dynarray_length(arr) - 1) * dynarray_stride(arr), dynarray_stride(arr));
    _dynarray_field_set(arr, LENGTH, dynarray_length(arr) - 1); // Decrement length.
}


