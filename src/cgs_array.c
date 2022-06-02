/* cgs_array.c
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 * cgs_array.c
 *
 * This file contains the source code of the libcgs implementation of a
 * generic dynamic array.
 *
 * After experimenting with an all-macro array implementation I decided to
 * go with a char-counting array.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

#include "cgs_array.h"

#include <stdlib.h>
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum cgs_array_constants {
	CGS_ARRAY_INITIAL_CAPACITY = 8,
	CGS_ARRAY_GROWTH_RATE = 2,
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Static Helpers - move to private header?
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static const void*
cgs_array_grow(struct cgs_array* a)
{
	size_t new_capacity = a->capacity == 0 ? CGS_ARRAY_INITIAL_CAPACITY
		: a->capacity * CGS_ARRAY_GROWTH_RATE;
	
	char* p = realloc(a->data, a->element_size * new_capacity);
        if (!p)
                return NULL;

        a->data = p;
        a->capacity = new_capacity;
	return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void*
cgs_array_new(struct cgs_array* a, size_t size)
{
        char* p = malloc(size * CGS_ARRAY_INITIAL_CAPACITY);
        if (!p)
                return NULL;

        a->length = 0;
        a->capacity = CGS_ARRAY_INITIAL_CAPACITY;
        a->element_size = size;
        a->data = p;

        return a;
}

void*
cgs_array_new_from_array(struct cgs_array* a, size_t size,
                const void* src, size_t len)
{
        char* p = malloc(len * size);
        if (!p)
                return NULL;

        memcpy(p, src, len * size);
        a->length = len;
        a->capacity = len;
        a->element_size = size;
        a->data = p;

        return a;
}

void
cgs_array_free(struct cgs_array* a)
{
        free(a->data);
}

void
cgs_array_free_all(struct cgs_array* a)
{
	if (a->data) {
		for (size_t i = 0, l = a->length; i < l; ++i)
			free(*(void**)cgs_array_get(a, i));
		free(a->data);
	}
}

void*
cgs_array_xfer(struct cgs_array* a, size_t* len)
{
	void* p = a->data;
	if (len)
		*len = a->length;

        memset(a, 0, sizeof(struct cgs_array));
	return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Inline Getter Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

size_t
cgs_array_length(const struct cgs_array* a);

void*
cgs_array_data(struct cgs_array* a);

const void*
cgs_array_get(const struct cgs_array* a, size_t index);

void*
cgs_array_get_mutable(struct cgs_array* a, size_t index);

const void*
cgs_array_begin(const struct cgs_array* a);

const void*
cgs_array_end(const struct cgs_array* a);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void*
cgs_array_push(struct cgs_array* a, const void* src)
{
	if (a->length == a->capacity && !cgs_array_grow(a))
		return NULL;

	char* dst = cgs_array_get_mutable(a, a->length);
	memcpy(dst, src, a->element_size);
	++a->length;
	return a;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Array Standard Algorithms
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void
cgs_array_sort(struct cgs_array* a, CgsCmp3Way cmp)
{
	qsort(a->data, a->length, a->element_size, cmp);
}

void*
cgs_array_find(struct cgs_array* a, const void* val, CgsCmp3Way cmp)
{
	for (size_t i = 0; i < a->length; ++i)
		if (cmp(cgs_array_get(a, i), val) == 0)
			return cgs_array_get_mutable(a, i);
	return NULL;
}

void
cgs_array_foreach(const struct cgs_array* a, CgsUnaryOp f, void* data)
{
        for (size_t i = 0, l = a->length; i < l; ++i)
                f(cgs_array_get(a, i), i, data);
}

void
cgs_array_transform(struct cgs_array* a, CgsUnaryOpMut f, void* data)
{
        for (size_t i = 0, l = a->length; i < l; ++i)
                f(cgs_array_get_mutable(a, i), i, data);
}

