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
 * Hash Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
/**
 * CgsHashFunc
 *
 * The expected signature of a hash function. The hash value is not adjusted
 * for the number of elements in the table.
 */
typedef size_t (*CgsHashFunc)(const void* key);

size_t
cgs_string_hash(const void* key);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_bucket
 *
 * A singly-linked list of buckets for managing collisions.
 *
 * @member key          An allocated string.
 * @member value        A cgs_variant containing the value. 
 * @member next         A single-linked list of collisions.
 */
struct cgs_bucket {
        char* key;
        struct cgs_variant value;
        struct cgs_bucket* next;
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
        struct cgs_bucket** table;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_hash_new
 *
 * A function to allocate and initialize a new hash table.
 *
 * Note: Hash and comparison functions are not required for this version. The
 * current implementation uses string keys so these functions are known.
 *
 * @param tab   A pointer to the hash table object to create.
 *
 * @return      A pointer to the hash table object on success or NULL on
 *              failure.
 */
void*
cgs_hash_new(struct cgs_hash* tab);

/**
 * cgs_hash_free
 *
 * A function to de-allocate a hash table.
 *
 * @param h     A pointer to the hash table object to deallocate.
 */
void
cgs_hash_free(struct cgs_hash* h);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
inline size_t
cgs_hash_length(const struct cgs_hash* h)
{
        return h->length;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_hash_lookup
 *
 * Searches the hash table for a given key and returns a read-only pointer to
 * the corresponding value if found.
 *
 * @param h     The hash table
 * @param key   The key to look up.
 *
 * @return      A read-only pointer to the value object if found or NULL if
 *              not found.
 */
const void*
cgs_hash_lookup(const struct cgs_hash* h, const char* key);

struct cgs_variant*
cgs_hash_get(struct cgs_hash* h, const char* key);

