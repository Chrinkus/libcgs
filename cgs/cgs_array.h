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

/**
 * struct cgs_array
 *
 * A generic, dynamic array
 */
struct cgs_array;

/**
 * cgs_array_new
 *
 * A convenience macro that calls 'sizeof' on a given type to call the
 * 'cgs_array_new_from size' function below.
 *
 * @param t	Type of array.
 */
#define cgs_array_new(t) cgs_array_new_from_size(sizeof(t))

/**
 * cgs_array_new_from_size
 *
 * Allocates a new array for elements the size of the given type's size.
 *
 * @param size	The size in bytes of the array type.
 *
 * @return	A new empty array.
 */
struct cgs_array*
cgs_array_new_from_size(size_t size);

/**
 * cgs_array_new_from_array
 *
 * Allocates a new array and fills it with the elements in src.
 *
 * @param src	The source array.
 * @param len	The length of the source array.
 * @param size	The size in bytes of the elements in src.
 *
 * @return	A new array with a copy of the elements in src.
 */
struct cgs_array*
cgs_array_new_from_array(const void* src, size_t len, size_t size);

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
 * @param a	The array of allocated elements to free.
 */
void
cgs_array_free_all(struct cgs_array* a);

/**
 * cgs_array_xfer
 *
 * Releases ownership of array memory. Frees the array structure.
 *
 * @param a	The array to transfer ownership from.
 * @param len	A pointer to a variable to store the length of the array in
 * 		or NULL.
 *
 * @return	A pointer to the transferred memory.
 */
void*
cgs_array_xfer(struct cgs_array* a, size_t* len);

/**
 * cgs_array_length
 *
 * Read the current length of an array.
 *
 * @param a	The array to read the length of.
 *
 * @return	The array length.
 */
size_t
cgs_array_length(const struct cgs_array* a);


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
const void*
cgs_array_get(const struct cgs_array* a, size_t index);

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
void*
cgs_array_get_mutable(struct cgs_array* a, size_t index);

/**
 * cgs_array_begin
 *
 * Get a read-only pointer to the first element in the array.
 *
 * @param a	The array.
 *
 * @return	A read-only pointer to the first element.
 */
const void*
cgs_array_begin(const struct cgs_array* a);

/**
 * cgs_array_end
 *
 * Get a read-only pointer to the element past the last element of the array.
 *
 * @param a	The array.
 *
 * @return	A read-only pointer to the element past the last element.
 */
const void*
cgs_array_end(const struct cgs_array* a);

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
 * @param val	A read-only pointer matching the element to find.
 * @param cmp	A three-way compare function for the elements of the array.
 *
 * @return	A pointer to the element if found or NULL if not found.
 */
void*
cgs_array_find(struct cgs_array* a, const void* val, CgsCmp3Way cmp);

