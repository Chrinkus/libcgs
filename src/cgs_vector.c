/* cgs_vector.c
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
 * cgs_vector.c
 *
 * This file contains the source code of the libcgs implementation of a
 * generic dynamic array.
 *
 * After experimenting with an all-macro vector implementation I decided to
 * go with a char-counting vector.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

#include "cgs_vector.h"

#include <stdlib.h>
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Vector Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum cgs_vector_constants {
	CGS_VECTOR_INITIAL_CAPACITY = 8,
	CGS_VECTOR_GROWTH_RATE = 2,
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Vector Static Helpers - move to private header?
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static const void*
cgs_vector_grow(struct cgs_vector* v)
{
	size_t new_capacity = v->capacity == 0 ? CGS_VECTOR_INITIAL_CAPACITY
		: v->capacity * CGS_VECTOR_GROWTH_RATE;
	
	char* p = realloc(v->data, v->element_size * new_capacity);
        if (!p)
                return NULL;

        v->data = p;
        v->capacity = new_capacity;
	return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Vector Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_vector
cgs_vector_new(size_t size)
{
        return (struct cgs_vector){
                .length = 0,
                .capacity = 0,
                .element_size = size,
                .data = NULL,
        };
}

void*
cgs_vector_copy(const struct cgs_vector* src, struct cgs_vector* dst)
{
        char* p = malloc(src->length * src->element_size);
        if (!p)
                return NULL;

        memcpy(p, src->data, src->length * src->element_size);
        dst->length = src->length;
        dst->capacity = src->capacity;
        dst->element_size = src->element_size;
        dst->data = p;

        return dst;
}

void*
cgs_vector_from_array(const void* arr, size_t len, size_t size,
                struct cgs_vector* v)
{
        char* p = malloc(len * size);
        if (!p)
                return NULL;

        memcpy(p, arr, len * size);
        v->length = len;
        v->capacity = len;
        v->element_size = size;
        v->data = p;

        return v;
}

void
cgs_vector_free(struct cgs_vector* v)
{
        free(v->data);
}

void
cgs_vector_free_all(struct cgs_vector* v)
{
	if (v->data) {
		for (size_t i = 0, l = v->length; i < l; ++i)
			free(*(void**)cgs_vector_get(v, i));
		free(v->data);
	}
}

void*
cgs_vector_xfer(struct cgs_vector* v, size_t* len)
{
	void* p = v->data;
	if (len)
		*len = v->length;

        memset(v, 0, sizeof(struct cgs_vector));
	return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Vector Inline Getter Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

size_t
cgs_vector_length(const struct cgs_vector* v);

const void*
cgs_vector_data(const struct cgs_vector* v);

void*
cgs_vector_data_mut(struct cgs_vector* v);

const void*
cgs_vector_get(const struct cgs_vector* v, size_t index);

void*
cgs_vector_get_mut(struct cgs_vector* v, size_t index);

const void*
cgs_vector_begin(const struct cgs_vector* v);

const void*
cgs_vector_end(const struct cgs_vector* v);

void
cgs_vector_clear(struct cgs_vector* v);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Vector Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void*
cgs_vector_push(struct cgs_vector* v, const void* src)
{
	if (v->length == v->capacity && !cgs_vector_grow(v))
		return NULL;

	char* dst = cgs_vector_get_mut(v, v->length);
	memcpy(dst, src, v->element_size);
	++v->length;
	return dst;
}

void
cgs_vector_remove(struct cgs_vector* v, size_t i)
{
        const size_t sz = v->element_size;

        --v->length;
        memmove(&v->data[i * sz], &v->data[(i+1) * sz], (v->length - i) * sz);
}

void
cgs_vector_remove_fast(struct cgs_vector* v, size_t i)
{
        const size_t sz = v->element_size;

        --v->length;
        memcpy(&v->data[i * sz], &v->data[v->length * sz], sz);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Vector Standard Algorithms
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void
cgs_vector_sort(struct cgs_vector* v, CgsCmp3Way cmp)
{
	qsort(v->data, v->length, v->element_size, cmp);
}

void*
cgs_vector_find(struct cgs_vector* v, CgsPredicate pred, const void* data)
{
	for (size_t i = 0; i < v->length; ++i)
		if (pred(cgs_vector_get(v, i), data))
			return cgs_vector_get_mut(v, i);
	return NULL;
}

void
cgs_vector_foreach(const struct cgs_vector* v, CgsUnaryOp f, void* data)
{
        for (size_t i = 0, l = v->length; i < l; ++i)
                f(cgs_vector_get(v, i), i, data);
}

void
cgs_vector_transform(struct cgs_vector* v, CgsUnaryOpMut f, void* data)
{
        for (size_t i = 0, l = v->length; i < l; ++i)
                f(cgs_vector_get_mut(v, i), i, data);
}

