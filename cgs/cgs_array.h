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

/**
 * Dynamic Array
 *
 * A dynamic vector implementation using macros. Yeah.
 */

enum cgs_array_scalars {
	CGS_ARRAY_INITIAL_MEMORY = 8,
	CGS_ARRAY_GROWTH_RATE = 2,
};

/**
 * CGS_ARRAY_DEFINE_STRUCT
 *
 * Defines an array struct of type TYPE called NAME. This ensures the array
 * type will have the expected members.
 */
#define CGS_ARRAY_DEFINE_STRUCT(name, type)	\
	 struct name {				\
		size_t len;			\
		size_t mem;			\
		type*  arr;			\
	} 

/**
 * cgs_array_init
 *
 * Initializes the array with a chunk of available memory.
 */
#define cgs_array_init(a)						\
	do {								\
		(a)->len = 0;						\
		(a)->mem = CGS_ARRAY_INITIAL_MEMORY;			\
		(a)->arr = malloc(sizeof(*(a)->arr) * (a)->mem);	\
	} while (0)

/**
 * cgs_array_free
 *
 * Frees the memory that the array occupies.
 */
#define cgs_array_free(a)		\
	do {				\
		(a)->len = 0;		\
		(a)->mem = 0;		\
		free((a)->arr);		\
	} while (0)

/**
 * cgs_array_xfer
 *
 * Passes ownership of the array memory. Sets arr to NULL but preserves len
 * for extraction.
 */
#define cgs_array_xfer(a, p)		\
	do {				\
		(p) = (a)->arr;		\
		(a)->arr = NULL;	\
		(a)->mem = 0;		\
	} while (0)

/**
 * cgs_array_place
 *
 * Sets the last value of the array. Assumes there is enough room. Called by
 * other macros, do not use.
 */
#define cgs_array_place(a, val) (a)->arr[(a)->len++] = (val)

/**
 * cgs_array_grow
 *
 * Increases the size of the array. Called by other macros, do not use.
 */
#define cgs_array_grow(a)						\
	do {								\
		size_t inc = (a)->mem * CGS_ARRAY_GROWTH_RATE;		\
		(a)->arr = realloc((a)->arr, sizeof(*(a)->arr) * inc);	\
		(a)->mem = inc;						\
	} while (0)

/**
 * cgs_array_push
 *
 * Add an element to the end of the array. Resize if necessary.
 */
#define cgs_array_push(a, val)			\
	do {					\
		if ((a)->len == (a)->mem)	\
			cgs_array_grow((a));	\
		cgs_array_place((a), (val));	\
	} while (0)

/**
 * cgs_array_get
 *
 * Get the value of the element at the requested index.
 */
#define cgs_array_get(a, index) (a)->arr[(index)]

/**
 * cgs_array_getp
 *
 * Get a pointer to the element at the requested index.
 */
#define cgs_array_getp(a, index) (&(a)->arr[(index)])

