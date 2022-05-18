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

/**
 * cgs_heap_new
 *
 * Allocate and initialize a new heap.
 *
 * @param heap  A pointer to the heap to allocate and initialize.
 * @param size  The size of the elements of the heap.
 * @param cmp   A pointer to a 3-way comparison function to use to order the
 *              elements of the heap.
 *
 * @return      A read-only pointer to the allocation to signal success or
 *              NULL on failure.
 */
const void*
cgs_heap_new(struct cgs_heap* heap, size_t size, CgsCmp3Way cmp);

/**
 * cgs_heap_free
 *
 * Deallocate a heap.
 *
 * @param heap  The heap to deallocate. Also resets all parameters to 0.
 */
void
cgs_heap_free(struct cgs_heap* heap);

