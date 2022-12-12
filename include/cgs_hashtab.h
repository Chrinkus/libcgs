/* cgs_hashtab.h
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
#include "cgs_variant.h"        // Users will need so include here
#include "cgs_defs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Struct Forward Declarations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct cgs_bucket;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
/**
 * CgsHashFunc
 *
 * The expected signature of a hash function. 
 */
typedef size_t (*CgsHashFunc)(const void* key, size_t size);

/**
 * cgs_string_hash
 *
 * Simple hash function for strings.
 *
 * @param key   A void pointer to a string to be hashed.
 * @param size  The number of buckets in the table to compress the hash to.
 *
 * @return      An unsigned value in the range of [0-size).
 */
size_t
cgs_string_hash(const void* key, size_t size);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum {
        CGS_HASHTAB_DEFAULT_SIZE = 31,
};

/**
 * struct cgs_hashtab
 *
 * A hash table implementation.
 *
 * @member length       The number of elements currently in the table.
 * @member table        The hash table.
 * @member hash         A function to hash the elements.
 * @member cmp          The function used for lookup matching.
 * @member size         The number of buckets in the table.
 * @member max_load     The highest the ratio of length to size is allowed to
 *                      get to before re-hashing.
 */
struct cgs_hashtab {
        size_t length;
        struct cgs_bucket** table;

        CgsHashFunc hash;
        CgsCmp3Way cmp;

        size_t size;
        double max_load;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_hashtab_new
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
cgs_hashtab_new(struct cgs_hashtab* tab);

/**
 * cgs_hashtab_free
 *
 * A function to de-allocate a hash table.
 *
 * @param h     A pointer to the hash table object to deallocate.
 */
void
cgs_hashtab_free(struct cgs_hashtab* h);

/**
 * cgs_hashtab_rehash
 *
 * Request a rehash of the table to the new size. The behaviour is dependent
 * on the requested size:
 *
 * - A request of 0 will trigger a rehash attempt using a size equal the next
 *   prime value that is larger than double the current size.
 * - A request equal-to or smaller than the current size will not trigger
 *   a rehash attempt.
 * - A request greater than the current size will trigger a rehash attempt.
 *
 * @param ht    The hash table.
 * @param size  The requested new size.
 *
 * @return      A pointer to the hash table if resized or NULL if not.
 */
void*
cgs_hashtab_rehash(struct cgs_hashtab* ht, size_t size);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Inline Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
inline size_t
cgs_hashtab_length(const struct cgs_hashtab* h)
{
        return h->length;
}

inline double
cgs_hashtab_current_load(const struct cgs_hashtab* h)
{
        return (double)h->length / (double)h->size;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_hashtab_lookup
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
cgs_hashtab_lookup(const struct cgs_hashtab* h, const char* key);

/**
 * cgs_hashtab_get
 *
 * Searches the hash table for a given key. If not found, creates a
 * new bucket and adds it to the table. Returns a writable pointer to the
 * variant containing the value or NULL if there was an error in creating a
 * new bucket.
 *
 * @param h     The hash table.
 * @param key   The key to get.
 *
 * @return      A writable pointer to the variant value on success or NULL on
 *              allocation error (bucket or key-dup).
 */
struct cgs_variant*
cgs_hashtab_get(struct cgs_hashtab* h, const char* key);

/**
 * cgs_hashtab_remove
 *
 * Searches the hash table for a given key. If found, removes the bucket from
 * the table. No error is indicated if the key is not found.
 *
 * @param h     The hash table.
 * @param key   The key of the value to remove.
 */
void
cgs_hashtab_remove(struct cgs_hashtab* h, const char* key);
