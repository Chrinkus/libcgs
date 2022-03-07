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

struct cgs_array {
	size_t length;
	size_t capacity;
	size_t element_size;
	char* memory;
};

enum cgs_array_constants {
	CGS_ARRAY_INITIAL_CAPACITY = 8,
	CGS_ARRAY_GROWTH_RATE = 2,
};

struct cgs_array*
cgs_array_new_from_size(size_t size)
{
	struct cgs_array* a = malloc(sizeof(struct cgs_array));
	if (!a)
		return NULL;

	a->element_size = size;
	a->capacity = CGS_ARRAY_INITIAL_CAPACITY;
	a->memory = malloc(a->element_size * a->capacity);
	if (!a->memory) {
		free(a);
		return NULL;
	}

	a->length = 0;
	return a;
}

struct cgs_array*
cgs_array_new_from_array(const void* src, size_t len, size_t size)
{
	struct cgs_array* a = malloc(sizeof(struct cgs_array));
	if (!a)
		return NULL;

	a->element_size = size;
	a->capacity = len;
	a->memory = malloc(a->element_size * a->capacity);
	if (!a->memory) {
		free(a);
		return NULL;
	}

	a->length = len;
	memcpy(a->memory, src, a->length * a->element_size);

	return a;
}

void
cgs_array_free(struct cgs_array* a)
{
	if (a) {
		free(a->memory);
		free(a);
	}
}

void*
cgs_array_xfer(struct cgs_array* a, size_t* len)
{
	void* p = a->memory;
	if (len)
		*len = a->length;
	free(a);

	return p;
}

size_t
cgs_array_length(const struct cgs_array* a)
{
	return a->length;
}

static const void*
cgs_array_grow(struct cgs_array* a)
{
	size_t new_capacity = a->capacity > 0
		? a->capacity * CGS_ARRAY_GROWTH_RATE
		: CGS_ARRAY_INITIAL_CAPACITY;
	
	char* p = realloc(a->memory, a->element_size * new_capacity);
	if (p) {
		a->memory = p;
		a->capacity = new_capacity;
	}
	return p;
}

const void*
cgs_array_get(const struct cgs_array* a, size_t index)
{
	return &a->memory[a->element_size * index];
}

void*
cgs_array_get_mutable(struct cgs_array* a, size_t index)
{
	return &a->memory[a->element_size * index];
}

void*
cgs_array_push(struct cgs_array* a, const void* src)
{
	if (a->length == a->capacity && !cgs_array_grow(a))
		return NULL;

	char* dst = cgs_array_get_mutable(a, a->length);
	memcpy(dst, src, a->element_size);
	++a->length;
	return dst;
}

void
cgs_array_sort(struct cgs_array* a, cgs_cmp_3way cmp)
{
	qsort(a->memory, a->length, a->element_size, cmp);
}

void*
cgs_array_find(struct cgs_array* a, const void* val, cgs_cmp_3way cmp)
{
	for (size_t i = 0; i < a->length; ++i)
		if (cmp(cgs_array_get(a, i), val) == 0)
			return cgs_array_get_mutable(a, i);
	return NULL;
}

