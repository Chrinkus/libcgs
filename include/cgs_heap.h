/* cgs_heap.h
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
 * Heap Public Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_heap
 *
 * A heap data structure.
 *
 * @member length       The number of elements in the heap.
 * @member capacity     The size of the allocation.
 * @member size         The size of the elements of the heap.
 * @member data         A pointer to the allocation.
 * @member cmp          A pointer to a 3-way comparison function to order the
 *                      elements of the heap.
 */
struct cgs_heap {
        size_t length;
        size_t capacity;
        size_t size;
        char* data;
        CgsCmp3Way cmp;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Heap Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_heap_new
 *
 * Allocate and initialize a new heap.
 *
 * @param size  The size of the elements of the heap.
 * @param cmp   A pointer to a 3-way comparison function to use to order the
 *              elements of the heap.
 *
 * @return      An empty, default-initialized heap structure.
 */
struct cgs_heap
cgs_heap_new(size_t size, CgsCmp3Way cmp);

/**
 * cgs_heap_free
 *
 * Deallocate a heap.
 *
 * @param ph    A void* to the heap to free. Matches standard library free
 *              function signature.
 */
void
cgs_heap_free(void* ph);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Heap Inline Getters, Etc..
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_heap_length
 *
 * Length getter.
 *
 * @param h     The heap.
 *
 * @return      The current length of the heap.
 */
inline size_t
cgs_heap_length(const struct cgs_heap* h)
{
        return h->length;
}

/**
 * cgs_heap_peek
 *
 * Get a read-only pointer to the first (highest priority) element in the
 * heap.
 *
 * @param h     The heap.
 *
 * @return      A read-only pointer to the first element.
 */
inline const void*
cgs_heap_peek(const struct cgs_heap* h)
{
        return h->length > 0 ? &h->data[0] : NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Heap Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_heap_push
 *
 * Add a value to a heap.
 *
 * @param h     The heap to add a value to.
 * @param val   A read-only pointer to a value to be copied and inserted.
 *
 * @return      A pointer to the heap on success or NULL on failure.
 */
void*
cgs_heap_push(struct cgs_heap* h, const void* val);

/**
 * cgs_heap_pop
 *
 * Remove the highest priority (first) item from the heap. Re-balance the
 * heap afterwards.
 *
 * @param h     The heap.
 * @param dst   A destination pointer to copy the priority value to.
 *
 * @return      A pointer to the heap if a value is available or NULL if the
 *              heap is empty.
 */
void*
cgs_heap_pop(struct cgs_heap* h, void* dest);

