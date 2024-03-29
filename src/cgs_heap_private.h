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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Heap Private Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum {
        CGS_HEAP_INITIAL_CAPACITY = 7,  // enough for a tree w/height of 3
        CGS_HEAP_DELTA = 2,             // heap growth rate
        CGS_HEAP_INC = 1,               // heap growth increment
        CGS_HEAP_SWAP = 1,              // extra space for swap temp
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Heap Private Function Notice
 *
 * All of these functions are for internal use or testing purposes only. A
 * user should only ever be concerned with the "top" element of a heap.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Heap Private Inline Getters, Etc
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_heap_parent
 *
 * FOR INTERNAL USE AND TESTING PURPOSES ONLY
 *
 * Get the parent index of a given heap index.
 *
 * @param i     The index to get the parent of.
 *
 * @return      The index of i's parent.
 */
inline ptrdiff_t
cgs_heap_parent(ptrdiff_t i)
{
        return (i - 1) / 2;
}

/**
 * cgs_heap_left
 *
 * FOR INTERNAL USE AND TESTING PURPOSES ONLY
 *
 * Get the 'left' child index of a given heap index.
 *
 * @param i     The index to get the left child index of.
 *
 * @return      The index of i's left child.
 */
inline size_t
cgs_heap_left(size_t i)
{
        return 2 * i + 1;
}

/**
 * cgs_heap_right
 *
 * FOR INTERNAL USE AND TESTING PURPOSES ONLY
 *
 * Get the 'right' child index of a given heap index.
 *
 * @param i     The index to get the right child index of.
 *
 * @return      The index of i's right child.
 */
inline size_t
cgs_heap_right(size_t i)
{
        return 2 * i + 2;
}

/**
 * cgs_heap_get
 *
 * FOR INTERNAL USE AND TESTING PURPOSES ONLY
 *
 * Get a read-only pointer to a given element within a heap.
 *
 * @param h     The heap.
 * @param i     The index of the element to retrieve.
 *
 * @return      A pointer to the element at the given index.
 */
inline const void*
cgs_heap_get(const struct cgs_heap* h, size_t i)
{
        return &h->data[h->size * i];
}

/**
 * cgs_heap_get_mut
 *
 * FOR INTERNAL USE AND TESTING PURPOSES ONLY
 *
 * Get a mutable pointer to a given element within a heap.
 *
 * @param h     The heap.
 * @param i     The index of the element to retrieve.
 *
 * @return      A pointer to the element at the given index.
 */
inline void*
cgs_heap_get_mut(struct cgs_heap* h, size_t i)
{
        return &h->data[h->size * i];
}

/**
 * cgs_heap_get_swapper
 *
 * FOR INTERNAL USE AND TESTING PURPOSES ONLY
 *
 * Get a mutable pointer to the swap element within a heap.
 *
 * @param h     The heap.
 *
 * @return      A pointer to the swap element [-1].
 */
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
        return old_capacity == 0
                ? CGS_HEAP_INITIAL_CAPACITY
                : old_capacity * CGS_HEAP_DELTA + CGS_HEAP_INC;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Heap Private Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_heap_cmp
 *
 * Sets up two elements to compare using the heap's 'cmp' ordering function.
 *
 * @param h     The heap.
 * @param i     The index of the 'left' comparison target.
 * @param j     The index of the 'right' comparison target.
 *
 * @return      An integer indicating the three-way result of the comparison.
 */
int
cgs_heap_cmp(const struct cgs_heap* h, size_t i, size_t j);

/**
 * cgs_heap_swap
 *
 * Swaps two elements in the heap. Used by '_sink' and '_swim' to reorder
 * the heap after '_pop's and '_push's. Utilizes the -1 indexed "swap pocket".
 *
 * @param h     The heap.
 * @param i     The index of an element to swap.
 * @param j     The index of an element to swap.
 */
void
cgs_heap_swap(struct cgs_heap* h, size_t i, size_t j);

/**
 * cgs_heap_sink
 *
 * Recursive function to sink low-priority elements to the bottom of the heap.
 * Called after a '_pop' swaps the lowest priority element to the top.
 *
 * @param h     The heap.
 * @param i     The index of the current element.
 */
void
cgs_heap_sink(struct cgs_heap* h, size_t i);

/**
 * cgs_heap_swim
 *
 * Raise a high-priority element towards the top of the heap. After a '_push'
 * adds a new element to the bottom of the heap, '_swim' is called to place
 * the element appropriately within the queue.
 *
 * @param h     The heap.
 * @param i     The index of the element that is "swimming".
 */
void
cgs_heap_swim(struct cgs_heap* h, ptrdiff_t i);

/**
 * cgs_heap_grow
 *
 * Grow the heap's allocation.
 *
 * @param h     The heap.
 *
 * @return      A pointer to the heap on success, NULL on failure.
 */
void*
cgs_heap_grow(struct cgs_heap* h);
