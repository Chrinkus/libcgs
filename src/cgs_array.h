/* cgs_array.h
 *
 * MIT License
 * 
 * Copyright (c) 2022 Chris Schick
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <stddef.h>
#include "cgs_defs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_array
 *
 * A generic, dynamic array
 *
 * @member length       The number of elements in the array.
 * @member capacity     The current number of elements that the array has
 *                      room for.
 * @member element_size The size of the elements in the array in bytes.
 * @member data         A pointer to the allocated memory.
 */
struct cgs_array {
	size_t length;
	size_t capacity;
	size_t element_size;
	char* data;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_array_new
 *
 * @param a     A pointer to an array object to initialize.
 * @param size  The size of the elements to be contained in the array.
 *
 * @return      A pointer to the array on successful allocation or NULL on
 *              failure.
 */
void*
cgs_array_new(struct cgs_array* a, size_t size);

/**
 * cgs_array_copy
 *
 * Copy an existing array into a new one. The 'dst' array should not own any
 * allocated memory.
 *
 * @param dst   The destination cgs_array to copy to.
 * @param src   The source cgs_array to copy from.
 *
 * @return      A pointer to the 'dst' array on success, NULL on failure.
 */
void*
cgs_array_copy(struct cgs_array* dst, const struct cgs_array* src);

/**
 * cgs_array_new_from_array
 *
 * Allocates a new array and fills it with the elements in src.
 *
 * @param a     A pointer to the array structure to create a copy to.
 * @param size	The size in bytes of the elements in the array.
 * @param src	A read-only pointer to the source array.
 * @param len	The length of the source array.
 *
 * @return      A pointer to the array on successful allocation or NULL on
 *              failure.
 */
void*
cgs_array_new_from_array(struct cgs_array* a, size_t size,
                const void* src, size_t len);

/**
 * cgs_array_free
 *
 * Deallocates an array.
 *
 * @param a	The array to deallocate.
 */
void
cgs_array_free(struct cgs_array* a);

/**
 * cgs_array_free_all
 *
 * Deallocates the elements of an array then the array itself.
 *
 * Warning: If the elements have a custom clean up function use
 * 'cgs_array_transform' instead then 'cgs_array_free' afterwards.
 *
 * @param a	The array of allocated elements to free.
 */
void
cgs_array_free_all(struct cgs_array* a);

/**
 * cgs_array_xfer
 *
 * Releases ownership of array memory.
 *
 * @param a	The array to transfer ownership from.
 * @param len	Optional pointer to a size_t variable to store the length of
 *              the array in or NULL.
 *
 * @return	A pointer to the transferred memory.
 */
void*
cgs_array_xfer(struct cgs_array* a, size_t* len);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_array_length
 *
 * Read the current length of an array.
 *
 * @param a	The array to read the length of.
 *
 * @return	The array length.
 */
inline size_t
cgs_array_length(const struct cgs_array* a)
{
	return a->length;
}

/**
 * cgs_array_data
 *
 * Get a read-only pointer to the array object.
 *
 * Warning: The 'data' member is cast to a const void* on return to alleviate
 * compiler warnings when assigning to the proper type.
 *
 * @param a     The array.
 *
 * @return      A const void-pointer to the data object.
 */
inline const void*
cgs_array_data(const struct cgs_array* a)
{
        return (const void*)a->data;
}

/**
 * cgs_array_data_mutable
 *
 * Get a writable pointer to the array object.
 *
 * Warning: The 'data' member is cast to a void* on return to alleviate
 * compiler warnings when assigning to the proper type.
 *
 * @param a     The array.
 *
 * @return      A void-pointer to the data object.
 */
inline void*
cgs_array_data_mutable(struct cgs_array* a)
{
        return (void*)a->data;
}

/**
 * cgs_array_get
 *
 * Get a read-only pointer to an element in the array. No bounds checking.
 *
 * @param a	The array.
 * @param index	The index of the element to get.
 *
 * @return	A read-only pointer to the element.
 */
inline const void*
cgs_array_get(const struct cgs_array* a, size_t index)
{
	return &a->data[a->element_size * index];
}

/**
 * cgs_array_get_mutable
 *
 * Get a mutable pointer to an element in the array. No bounds checking.
 *
 * @param a	The array.
 * @param index	The index of the element to get.
 *
 * @return	A mutable pointer to the element.
 */
inline void*
cgs_array_get_mutable(struct cgs_array* a, size_t index)
{
	return &a->data[a->element_size * index];
}

/**
 * cgs_array_begin
 *
 * Get a read-only pointer to the first element in the array.
 *
 * @param a	The array.
 *
 * @return	A read-only pointer to the first element.
 */
inline const void*
cgs_array_begin(const struct cgs_array* a)
{
	return a->data;
}

/**
 * cgs_array_end
 *
 * Get a read-only pointer to the element past the last element of the array.
 *
 * @param a	The array.
 *
 * @return	A read-only pointer to the element past the last element.
 */
inline const void*
cgs_array_end(const struct cgs_array* a)
{
	return a->data + a->length * a->element_size;
}

/**
 * cgs_array_clear
 *
 * Prepare an existing array for re-use by setting length to zero. Does not
 * deallocate memory.
 *
 * **SECURITY WARNING**
 * - Does not erase the old contents of the array!
 * - To completely erase the old contents of the array call cgs_array_erase.
 *
 * @param a     The array.
 */
inline void
cgs_array_clear(struct cgs_array* a)
{
        a->length = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_array_push
 *
 * Add an element to the end of the array. May invalidate existing pointers
 * to elements.
 *
 * @param a	The array.
 * @param src	A read-only pointer to the element to add.
 *
 * @return	A mutable pointer to the element added.
 */
void*
cgs_array_push(struct cgs_array* a, const void* src);

/**
 * cgs_array_remove
 *
 * Remove an element at the given index in an array. Preserves order. No
 * bounds checking.
 *
 * @param a     The array.
 * @param i     The index of the element in the array to remove.
 */
void
cgs_array_remove(struct cgs_array* a, size_t i);

/**
 * cgs_array_remove_fast
 *
 * Remove an element at the given index in an array. Affects order by moving
 * last element to given index. No bounds checking.
 *
 * @param a     The array.
 * @param i     The index of the element in the array to remove.
 */
void
cgs_array_remove_fast(struct cgs_array* a, size_t i);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Standard Algorithms
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_array_sort
 *
 * Sort an array in-place.
 *
 * @param a	The array.
 * @param cmp	A three-way compare function for the elements of the array.
 */
void
cgs_array_sort(struct cgs_array* a, CgsCmp3Way cmp);

/**
 * cgs_array_find
 *
 * Find an element in an array.
 *
 * @param a	The array.
 * @param pred  A comparison function that will be passed an element and the
 *              optional userdata parameter.
 * @param data  A pointer to userdata that will be passed as the second
 *              argument to the compare function.
 *
 * @return	A pointer to the element if found or NULL if not found.
 */
void*
cgs_array_find(struct cgs_array* a, CgsPredicate pred, const void* data);

/**
 * cgs_array_foreach
 *
 * Traverse a read-only array and perform an operation using each of its
 * elements.
 *
 * @param a     A read-only pointer to the array.
 * @param f     A function to perform taking an element, the current index,
 *              and a pointer to userdata.
 * @param data  The userdata.
 */
void
cgs_array_foreach(const struct cgs_array* a, CgsUnaryOp f, void* data);

/**
 * cgs_array_transform
 *
 * Traverse an array and perform an operation on each element, possibly
 * altering their contents.
 *
 * @param a     A pointer to the array.
 * @param f     A function to perform taking an element, the current index,
 *              and a pointer to userdata.
 * @param data  The userdata.
 */
void
cgs_array_transform(struct cgs_array* a, CgsUnaryOpMut f, void* data);

