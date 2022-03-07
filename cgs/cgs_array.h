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
#include "cgs_compare.h"

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
 * @param element_size	The size in bytes of the array type.
 *
 * @return		A new empty array.
 */
struct cgs_array*
cgs_array_new_from_size(size_t element_size);

/**
 * cgs_array_free
 *
 * Deallocates an array.
 *
 * @param a	The array to deallocate.
 */
void
cgs_array_free(struct cgs_array* a);

size_t
cgs_array_length(const struct cgs_array* a);


const void*
cgs_array_get(const struct cgs_array* a, size_t index);

void*
cgs_array_get_mutable(struct cgs_array* a, size_t index);

void*
cgs_array_push(struct cgs_array* a, const void* src);

void
cgs_array_sort(struct cgs_array* a, cgs_cmp_3way cmp);

