/* cgs_iter.c
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
#include "cgs_iter.h"

#include "cgs_vector.h"

enum cgs_iter_type {
        ITER_VEC,
        ITER_VEC_MUT,
};

static const void*
iter_vec_next(struct cgs_iter_vec* it)
{
        if (it->cur == it->end)
                return NULL;

        const void* p = it->cur;
        it->cur += it->size;
        return p;
}

static void*
iter_vec_mut_next(struct cgs_iter_vec_mut* it)
{
        if (it->cur == it->end)
                return NULL;
        void* p = it->cur;
        it->cur += it->size;
        return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Public Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const void*
cgs_iter_next(struct cgs_iter* it)
{
        switch (it->type) {
        case ITER_VEC:          return iter_vec_next(&it->data.vec);
        default:                return NULL;
        }
}

void*
cgs_iter_mut_next(struct cgs_iter* it)
{
        switch (it->type) {
        case ITER_VEC_MUT:      return iter_vec_mut_next(&it->data.vec_mut);
        default:                return NULL;
        }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Vector Iterators
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_iter
cgs_vector_iter(const struct cgs_vector* v)
{
        return (struct cgs_iter) {
                .type = ITER_VEC,
                .data.vec = (struct cgs_iter_vec) {
                        .cur = v->data,
                        .end = &v->data[v->length * v->element_size],
                        .size = v->element_size,
                },
        };
}

struct cgs_iter
cgs_vector_iter_mut(struct cgs_vector* v)
{
        return (struct cgs_iter) {
                .type = ITER_VEC_MUT,
                .data.vec_mut = (struct cgs_iter_vec_mut) {
                        .cur = v->data,
                        .end = v->data
                                ? &v->data[v->length * v->element_size]
                                : NULL,
                        .size = v->element_size,
                },
        };
}
