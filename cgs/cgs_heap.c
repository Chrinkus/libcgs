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

#include <stdlib.h>     // malloc, free
#include <string.h>     // memset

enum { CGS_HEAP_INITIAL_CAPACITY = 15, };

const void*
cgs_heap_new(struct cgs_heap* heap, size_t size, CgsCmp3Way cmp)
{
        char* p = malloc(size * CGS_HEAP_INITIAL_CAPACITY);
        if (!p)
                return NULL;

        heap->length = 0;
        heap->capacity = CGS_HEAP_INITIAL_CAPACITY;
        heap->size = size;
        heap->data = p;
        heap->cmp = cmp;

        return (const void*)p;
}

void
cgs_heap_free(struct cgs_heap* heap)
{
        if (!heap)
                return;

        free(heap->data);
        memset(heap, 0, sizeof(struct cgs_heap));
}

