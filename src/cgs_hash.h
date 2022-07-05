/* cgs_hash.h
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
#include "cgs_variant.h"
#include "cgs_defs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * CgsHashFunc
 *
 * The expected signature of a hash function.
 */
typedef size_t (*CgsHashFunc)(const struct cgs_variant* var, size_t size);

struct cgs_list {
        void* key;
        struct cgs_variant data;
        struct cgs_list* next;
};

/**
 * struct cgs_hash
 *
 * A hash table implementation.
 *
 * @member length       The number of elements currently in the table.
 * @member size         The number of buckets in the table.
 * @member hash         A function to hash the elements.
 * @member cmp          The function used for lookup matching.
 * @member table        The hash table.
 */
struct cgs_hash {
        size_t length;
        size_t size;
        CgsHashFunc hash;
        CgsCmp3Way cmp;
        struct cgs_list** table;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
size_t cgs_string_hash(const struct cgs_variant* var, size_t size);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_hash_new
 *
 * A function to allocate and initialize a new hash table.
 *
 * @param tab   A pointer to the hash table object to create.
 * @param hash  The function to use for the elements.
 * @param cmp   The function used for lookup matching.
 *
 * @return      A pointer to the hash table object on success or NULL on
 *              failure.
 */
void*
cgs_hash_new(struct cgs_hash* tab, CgsHashFunc hash, CgsCmp3Way cmp);

/**
 * cgs_hash_free
 *
 * A function to de-allocate a hash table.
 *
 * @param h     A pointer to the hash table object to deallocate.
 */
void
cgs_hash_free(struct cgs_hash* h);

