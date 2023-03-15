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
#include "cgs_variant.h"
#include "cgs_defs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_htab_bucket
 *
 * FORWARD DECLARATION ONLY
 *
 * There is no need for a user to work with buckets.
 */
struct cgs_htab_bucket;

/**
 * struct cgs_hashtab
 *
 * A hash table implementation.
 *
 * @member length       The number of elements currently in the table.
 * @member table        The hash table.
 * @member hash         A function to hash the elements.
 * @member ff           A function used to free the elements, if necessary.
 * @member cmp          The function used for lookup matching.
 * @member size         The number of buckets in the table.
 * @member max_load     The highest the ratio of length to size is allowed to
 *                      get to before re-hashing.
 */
struct cgs_hashtab {
        size_t length;
        struct cgs_htab_bucket** table;

        CgsHashFunc hash;
        CgsFreeFunc ff;
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
 * Create a new, empty, unallocated hash table.
 *
 * Note: Hash and comparison functions are not required for this version. The
 * current implementation uses string keys so these functions are known.
 *
 * @param ff    A function to use to free the elements if necessary or NULL.
 *
 * @return      An empty hash table.
 */
struct cgs_hashtab
cgs_hashtab_new(CgsFreeFunc ff);

/**
 * cgs_hashtab_free
 *
 * A function to de-allocate a hash table.
 *
 * @param p     A pointer to the hash table object to deallocate. Passed as
 *              void* to match standard library free.
 */
void
cgs_hashtab_free(void* p);

/**
 * cgs_hashtab_reserve
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
cgs_hashtab_reserve(struct cgs_hashtab* ht, size_t size);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Inline Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_hashtab_length
 *
 * Get the length of a hash table.
 *
 * @param ht    The hash table.
 *
 * @return      The length of the hash table.
 */
inline size_t
cgs_hashtab_length(const struct cgs_hashtab* ht)
{
        return ht->length;
}

/**
 * cgs_hashtab_current_load
 *
 * Get the current load factor of the hash table.
 *
 * @param ht    The hash table.
 *
 * @return      A floating point value representing the ratio of elements to
 *              buckets in the table.
 */
inline double
cgs_hashtab_current_load(const struct cgs_hashtab* ht)
{
        return (double)ht->length / (double)ht->size;
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
 * cgs_hashtab_lookup_mut
 *
 * Searches the hash table for a given key and returns a mutable pointer to
 * the corresponding value if found.
 *
 * @param h     The hash table
 * @param key   The key to look up.
 *
 * @return      A mutable pointer to the value object if found or NULL if
 *              not found.
 */
void*
cgs_hashtab_lookup_mut(struct cgs_hashtab* ht, const char* key);

/**
 * cgs_hashtab_insert
 *
 * Insert a key/value pair into the hash table. The value may be optionally
 * passed for copy insertion. A pointer to the variant is returned so the
 * value may be explicitly set after.
 *
 * Returns NULL if an element of the given key already exists in the table.
 *
 * @param ht    The hash table.
 * @param key   The key of the element to insert.
 * @param var   A read-only pointer to a variant to copy as the value.
 *
 * @return      A mutable pointer to the variant value on successful insertion,
 *              NULL on failed insertion.
 */
struct cgs_variant*
cgs_hashtab_insert(struct cgs_hashtab* ht, const char* key,
                const struct cgs_variant* var);

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Iterator
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct cgs_hashtab_iter_mut {
        struct cgs_htab_bucket** tab;
        struct cgs_htab_bucket** end;
        struct cgs_htab_bucket*  cur;
};

struct cgs_hashtab_iter_mut
cgs_hashtab_begin_mut(struct cgs_hashtab* ht);

void*
cgs_hashtab_iter_mut_next(struct cgs_hashtab_iter_mut* it);

struct cgs_variant*
cgs_hashtab_iter_mut_get(struct cgs_hashtab_iter_mut* it);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_hash_str
 *
 * Simple hash function for strings.
 *
 * @param key   A void pointer to a string to be hashed.
 * @param size  The number of buckets in the table to compress the hash to.
 *
 * @return      An unsigned value in the range of [0-size).
 */
size_t
cgs_hash_str(const void* key, size_t size);
