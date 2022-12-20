/* cgs_heap.c
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
 * cgs_heap.c info
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

#include "cgs_heap.h"
#include "cgs_heap_private.h"

#include <stdlib.h>     // malloc, free
#include <string.h>     // memset, memcpy

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Inline symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

size_t
cgs_heap_length(const struct cgs_heap* h);

const void*
cgs_heap_peek(const struct cgs_heap* h);

ptrdiff_t
cgs_heap_parent(ptrdiff_t i);

size_t
cgs_heap_left(size_t i);

size_t
cgs_heap_right(size_t i);

const void*
cgs_heap_get(const struct cgs_heap* h, size_t i);

void*
cgs_heap_get_mut(struct cgs_heap* h, size_t i);

void*
cgs_heap_get_swapper(struct cgs_heap* h);

size_t
cgs_heap_new_capacity(size_t old_capacity);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Private helper functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int
cgs_heap_cmp(const struct cgs_heap* h, size_t i, size_t j)
{
        const void* a = cgs_heap_get(h, i);
        const void* b = cgs_heap_get(h, j);

        return h->cmp(a, b);
}

void
cgs_heap_swap(struct cgs_heap* h, size_t i, size_t j)
{
        void* t = cgs_heap_get_swapper(h);
        void* a = cgs_heap_get_mut(h, i);
        void* b = cgs_heap_get_mut(h, j);

        memcpy(t, a, h->size);
        memcpy(a, b, h->size);
        memcpy(b, t, h->size);
}

void
cgs_heap_sink(struct cgs_heap* h, size_t i)
{
        size_t l = cgs_heap_left(i);
        size_t prio = l < h->length && cgs_heap_cmp(h, l, i) < 0 ? l : i;

        size_t r = cgs_heap_right(i);
        if (r < h->length && cgs_heap_cmp(h, r, prio) < 0)
                prio = r;

        if (prio != i) {
                cgs_heap_swap(h, i, prio);
                cgs_heap_sink(h, prio);
        }
}

void*
cgs_heap_grow(struct cgs_heap* h)
{
        size_t new_capacity = cgs_heap_new_capacity(h->capacity);
        char* p = malloc((new_capacity + CGS_HEAP_SWAP) * h->size);
        if (!p)
                return NULL;

        if (h->data) {
                h->data -= h->size;     // backup to catch old pocket
                memcpy(p, h->data, h->size * (h->length + CGS_HEAP_SWAP));
                free(h->data);
        }

        p += h->size;                   // stash new swap pocket
        h->data = p;
        h->capacity = new_capacity;
        return h;
}

void
cgs_heap_swim(struct cgs_heap* h, ptrdiff_t i)
{
        for (ptrdiff_t parent = cgs_heap_parent(i);
                        i > 0 && cgs_heap_cmp(h, parent, i) > 0;
                        i = parent, parent = cgs_heap_parent(i))
                cgs_heap_swap(h, i, parent);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Public API functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct cgs_heap
cgs_heap_new(size_t size, CgsCmp3Way cmp)
{
        return (struct cgs_heap){
                .length = 0,
                .capacity = 0,
                .size = size,
                .data = NULL,
                .cmp = cmp,
        };
}

void
cgs_heap_free(void* ph)
{
        struct cgs_heap* heap = ph;
        if (!heap || !heap->data)
                return;

        free(heap->data - heap->size);  // free -1 as well
        memset(heap, 0, sizeof(struct cgs_heap));
}

const void*
cgs_heap_push(struct cgs_heap* h, const void* val)
{
        if (h->length == h->capacity && !cgs_heap_grow(h))
                return NULL;

        memcpy(&h->data[h->length * h->size], val, h->size);

        cgs_heap_swim(h, h->length);
        ++h->length;

        return h->data;
}

const void*
cgs_heap_pop(struct cgs_heap* h, void* dest)
{
        if (h->length == 0)
                return NULL;

        --h->length;
        memcpy(dest, &h->data[0], h->size);
        memcpy(&h->data[0], &h->data[h->length * h->size], h->size);
        cgs_heap_sink(h, 0);

        return h->data;
}

