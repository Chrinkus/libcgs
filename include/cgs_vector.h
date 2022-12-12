/* cgs_vector.h
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
 * Vector Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_vector
 *
 * A generic, dynamic array
 *
 * @member length       The number of elements in the vector.
 * @member capacity     The current number of elements that the vector has
 *                      room for.
 * @member element_size The size of the elements in the vector in bytes.
 * @member data         A pointer to the allocated memory.
 */
struct cgs_vector {
	size_t length;
	size_t capacity;
	size_t element_size;
	char* data;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Vector Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_vector_new
 *
 * @param size  The size of the elements to be contained in the vector.
 *
 * @return      An empty vector object initialized for elements of the
 *              given size.
 */
struct cgs_vector
cgs_vector_new(size_t size);

/**
 * cgs_vector_copy
 *
 * Copy an existing vector into a new one. The 'dst' vector should not own any
 * allocated memory, ideally being created by the call `cgs_vector_new(0)`.
 *
 * @param src   The source cgs_vector to copy from.
 * @param dst   The destination cgs_vector to copy to.
 *
 * @return      A pointer to the 'dst' vector on success, NULL on failure.
 */
void*
cgs_vector_copy(const struct cgs_vector* src, struct cgs_vector* dst);

/**
 * cgs_vector_from_array
 *
 * Allocates a new vector and fills it with the elements in src.
 *
 * @param arr	A read-only pointer to the source array.
 * @param len	The length of the source array.
 * @param size	The size in bytes of the elements in the vector.
 * @param v     A pointer to the vector structure to create a copy to.
 *
 * @return      A pointer to the vector on successful allocation or NULL on
 *              failure.
 */
void*
cgs_vector_from_array(const void* arr, size_t len, size_t size,
                struct cgs_vector* v);

/**
 * cgs_vector_free
 *
 * Deallocates an vector.
 *
 * @param v	The vector to deallocate.
 */
void
cgs_vector_free(struct cgs_vector* v);

/**
 * cgs_vector_free_all
 *
 * Deallocates the elements of an vector then the vector itself.
 *
 * Warning: If the elements have a custom clean up function use
 * 'cgs_vector_transform' instead then 'cgs_vector_free' afterwards.
 *
 * @param v	The vector of allocated elements to free.
 */
void
cgs_vector_free_all(struct cgs_vector* v);

/**
 * cgs_vector_free_all_with
 *
 * Deallocates the elements of a vector using the provided function, then 
 * frees the vector itself.
 *
 * @param v     The vector of allocated elements to free.
 * @param ff    The custom free-ing function.
 */
void
cgs_vector_free_all_with(struct cgs_vector* v, CgsFreeFunc ff);

/**
 * cgs_vector_xfer
 *
 * Releases ownership of vector memory.
 *
 * @param v	The vector to transfer ownership from.
 * @param len	Optional pointer to a size_t variable to store the length of
 *              the vector in or NULL.
 *
 * @return	A pointer to the transferred memory.
 */
void*
cgs_vector_xfer(struct cgs_vector* v, size_t* len);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_vector_length
 *
 * Read the current length of an vector.
 *
 * @param v	The vector to read the length of.
 *
 * @return	The vector length.
 */
inline size_t
cgs_vector_length(const struct cgs_vector* v)
{
	return v->length;
}

/**
 * cgs_vector_data
 *
 * Get a read-only pointer to the vector object.
 *
 * Warning: The 'data' member is cast to a const void* on return to alleviate
 * compiler warnings when assigning to the proper type.
 *
 * @param v     The vector.
 *
 * @return      A const void-pointer to the data object.
 */
inline const void*
cgs_vector_data(const struct cgs_vector* v)
{
        return (const void*)v->data;
}

/**
 * cgs_vector_data_mut
 *
 * Get a writable pointer to the vector object.
 *
 * Warning: The 'data' member is cast to a void* on return to alleviate
 * compiler warnings when assigning to the proper type.
 *
 * @param v     The vector.
 *
 * @return      A void-pointer to the data object.
 */
inline void*
cgs_vector_data_mut(struct cgs_vector* v)
{
        return (void*)v->data;
}

/**
 * cgs_vector_get
 *
 * Get a read-only pointer to an element in the vector. No bounds checking.
 *
 * @param v	The vector.
 * @param index	The index of the element to get.
 *
 * @return	A read-only pointer to the element.
 */
inline const void*
cgs_vector_get(const struct cgs_vector* v, size_t index)
{
	return &v->data[v->element_size * index];
}

/**
 * cgs_vector_get_mut
 *
 * Get a mutable pointer to an element in the vector. No bounds checking.
 *
 * @param v	The vector.
 * @param index	The index of the element to get.
 *
 * @return	A mutable pointer to the element.
 */
inline void*
cgs_vector_get_mut(struct cgs_vector* v, size_t index)
{
	return &v->data[v->element_size * index];
}

inline const void*
cgs_vector_first(const struct cgs_vector* v)
{
        return v->data;
}

inline const void*
cgs_vector_last(const struct cgs_vector* v)
{
        return &v->data[v->element_size * (v->length - 1)];
}

/**
 * cgs_vector_begin
 *
 * Get a read-only pointer to the first element in the vector.
 *
 * @param a	The vector.
 *
 * @return	A read-only pointer to the first element.
 */
inline const void*
cgs_vector_begin(const struct cgs_vector* v)
{
	return v->data;
}

/**
 * cgs_vector_end
 *
 * Get a read-only pointer to the element past the last element of the vector.
 *
 * @param v	The vector.
 *
 * @return	A read-only pointer to the element past the last element.
 */
inline const void*
cgs_vector_end(const struct cgs_vector* v)
{
	return v->data + v->length * v->element_size;
}

/**
 * cgs_vector_clear
 *
 * Prepare an existing vector for re-use by setting length to zero. Does not
 * deallocate memory.
 *
 * **SECURITY WARNING**
 * - Does not erase the old contents of the vector!
 * - To completely erase the old contents of the vector call cgs_vector_erase.
 *
 * @param v     The vector.
 */
inline void
cgs_vector_clear(struct cgs_vector* v)
{
        v->length = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_vector_push
 *
 * Add an element to the end of the vector. May invalidate existing pointers
 * to elements.
 *
 * @param v	The vector.
 * @param src	A read-only pointer to the element to add.
 *
 * @return	A mutable pointer to the element added.
 */
void*
cgs_vector_push(struct cgs_vector* v, const void* src);

/**
 * cgs_vector_pop
 *
 * Remove the last element of the vector, copying it into the provided
 * memory location.
 *
 * @param v     The vector.
 * @param p     A pointer to the copy destination.
 *
 * @return      A pointer back to p on successful pop, NULL when length is
 *              zero.
 */
void*
cgs_vector_pop(struct cgs_vector* v, void* p);

/**
 * cgs_vector_remove
 *
 * Remove an element at the given index in an vector. Preserves order. No
 * bounds checking.
 *
 * @param v     The vector.
 * @param i     The index of the element in the vector to remove.
 */
void
cgs_vector_remove(struct cgs_vector* v, size_t i);

/**
 * cgs_vector_remove_fast
 *
 * Remove an element at the given index in an vector. Affects order by moving
 * last element to given index. No bounds checking.
 *
 * @param v     The vector.
 * @param i     The index of the element in the vector to remove.
 */
void
cgs_vector_remove_fast(struct cgs_vector* v, size_t i);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Standard Algorithms
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_vector_sort
 *
 * Sort an vector in-place.
 *
 * @param v	The vector.
 * @param cmp	A three-way compare function for the elements of the vector.
 */
void
cgs_vector_sort(struct cgs_vector* v, CgsCmp3Way cmp);

/**
 * cgs_vector_find
 *
 * Find an element in an vector.
 *
 * @param v	The vector.
 * @param pred  A comparison function that will be passed an element and the
 *              optional userdata parameter.
 * @param data  A pointer to userdata that will be passed as the second
 *              argument to the compare function.
 *
 * @return	A pointer to the element if found or NULL if not found.
 */
void*
cgs_vector_find(struct cgs_vector* v, CgsPredicate pred, const void* data);

/**
 * cgs_vector_foreach
 *
 * Traverse a read-only vector and perform an operation using each of its
 * elements.
 *
 * @param v     A read-only pointer to the vector.
 * @param f     A function to perform taking an element, the current index,
 *              and a pointer to userdata.
 * @param data  The userdata.
 */
void
cgs_vector_foreach(const struct cgs_vector* v, CgsUnaryOp f, void* data);

/**
 * cgs_vector_transform
 *
 * Traverse an vector and perform an operation on each element, possibly
 * altering their contents.
 *
 * @param v     A pointer to the vector.
 * @param f     A function to perform taking an element, the current index,
 *              and a pointer to userdata.
 * @param data  The userdata.
 */
void
cgs_vector_transform(struct cgs_vector* v, CgsUnaryOpMut f, void* data);

