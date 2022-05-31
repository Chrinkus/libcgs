/* cgs_heap_private.h
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

#include "cgs_heap.h"

enum {
        CGS_HEAP_INITIAL_CAPACITY = 7,  // enough for a tree w/height of 3
        CGS_HEAP_DELTA = 2,             // heap growth rate
        CGS_HEAP_INC = 1,               // heap growth increment
        CGS_HEAP_SWAP = 1,              // extra space for swap temp
};

inline ptrdiff_t
cgs_heap_parent(ptrdiff_t i)
{
        return (i - 1) / 2;
}

inline size_t
cgs_heap_left(size_t i)
{
        return 2 * i + 1;
}

inline size_t
cgs_heap_right(size_t i)
{
        return 2 * i + 2;
}

inline const void*
cgs_heap_get(const struct cgs_heap* h, size_t i)
{
        return &h->data[h->size * i];
}

inline void*
cgs_heap_get_mutable(struct cgs_heap* h, size_t i)
{
        return &h->data[h->size * i];
}

inline void*
cgs_heap_get_swapper(struct cgs_heap* h)
{
        return &h->data[h->size * -1];
}

/**
 * cgs_heap_new_capacity
 *
 * Calculate a new capacity off of old. Each allocation adds a new "layer" to
 * the inner binary tree so the sizes will grow as such:
 *
 *      3 > 7 > 15 > 31
 * 
 * The relationship is old_capacity * 2 + 1. Enumerated constants have been
 * created resulting in:
 *
 *      old_capacity * CGS_HEAP_DELTA + CGS_HEAP_INC
 *
 * @param old_capacity  The previous capacity. This value is expected to be
 *                      a "max" binary value.
 *
 * @return              The next "max" binary value.
 */
inline size_t
cgs_heap_new_capacity(size_t old_capacity)
{
        return old_capacity * CGS_HEAP_DELTA + CGS_HEAP_INC;
}

const void*
cgs_heap_grow(struct cgs_heap* h);

void
cgs_heap_swim(struct cgs_heap* h, ptrdiff_t i);

