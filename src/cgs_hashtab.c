/* cgs_hashtab.c
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
#include "cgs_hashtab.h"
#include "cgs_compare.h"
#include "cgs_string_utils.h"
#include "cgs_numeric.h"

#include <stdlib.h>
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum hash_table_constants {
        HTAB_DEFAULT_SIZE = 31,
        HTAB_BUCKET_PSIZE = sizeof(struct cgs_htab_bucket*),
        HTAB_INITIAL_ALLOC = HTAB_DEFAULT_SIZE * HTAB_BUCKET_PSIZE,
};

const double HTAB_DEFAULT_LOAD_FACTOR = 0.8;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Private Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_htab_bucket
 *
 * A singly-linked list of buckets for managing collisions.
 *
 * @member key          An allocated string.
 * @member value        A cgs_variant containing the value. 
 * @member next         A single-linked list of collisions.
 */
struct cgs_htab_bucket {
        char* key;
        struct cgs_variant value;
        struct cgs_htab_bucket* next;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Bucket Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_htab_bucket_new
 *
 * Allocates a new bucket and key-string.
 * 
 * @param key   The string to use as a key.
 *
 * @return      A pointer to the newly allocated bucket on success or NULL
 *              on failure.
 */
static struct cgs_htab_bucket*
cgs_htab_bucket_new(const char* key)
{
        struct cgs_htab_bucket* b = malloc(sizeof(struct cgs_htab_bucket));
        if (!b)
                return NULL;
        char* p = cgs_strdup(key);
        if (!p) {
                free(b);
                return NULL;
        }
        b->key = p;
        return b;
}

/**
 * cgs_htab_bucket_free
 *
 * Frees the memory allocated to a bucket, its key and any memory allocated
 * to the variant value.
 *
 * @param b     The bucket to free.
 */
static void
cgs_htab_bucket_free(void* p, CgsFreeFunc ff)
{
        if (!p)
                return;

        struct cgs_htab_bucket* b = p;

        cgs_htab_bucket_free(b->next, ff);
        cgs_variant_free(&b->value, ff);
        free(b->key);
        free(b);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Private Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * init_null_buckets
 *
 * Set all elements of an array of bucket pointers to NULL.
 *
 * @param ppb   A pointer to an array of bucket pointers.
 * @param size  The number of elements in the array.
 */
static inline void
init_null_buckets(struct cgs_htab_bucket** ppb, size_t size)
{
        for (size_t i = 0; i < size; ++i)
                ppb[i] = NULL;
}

/**
 * hashtab_build
 *
 * Perform the initial allocation of an empty hash table.
 *
 * @param ht    The newly created hash table.
 *
 * @return      A pointer to the hash table on success, NULL on failure.
 */
static void*
hashtab_build(struct cgs_hashtab* ht)
{
        struct cgs_htab_bucket** ppb = malloc(HTAB_INITIAL_ALLOC);
        if (!ppb)
                return NULL;

        init_null_buckets(ppb, HTAB_DEFAULT_SIZE);

        ht->table = ppb;
        ht->size = HTAB_DEFAULT_SIZE;

        return ht;
}

/**
 * hashtab_rehash
 *
 * Rehash the elements of a hash table into the provided new bucket-pointer
 * array. Frees the old array allocation.
 *
 * @param ht            The hash table.
 * @param new_tab       The new bucket-pointer array.
 * @param new_size      The size of the new allocation.
 */
static void
hashtab_rehash(struct cgs_hashtab* ht, struct cgs_htab_bucket** new_htab,
                size_t new_size)
{
        for (size_t i = 0; i < ht->size; ++i) {
                struct cgs_htab_bucket* bp = ht->table[i];
                while (bp) {
                        struct cgs_htab_bucket* tmp = bp->next;
                        size_t hash = ht->hash(bp->key, new_size);
                        bp->next = new_htab[hash];
                        new_htab[hash] = bp;
                        bp = tmp;
                }
        }

        free(ht->table);
        ht->table = new_htab;
        ht->size = new_size;
}

/**
 * hashtab_grow
 *
 * Increase the number of buckets in a hash table.
 *
 * @param ht    The hash table.
 * @param size  The desired minimum size. The allocation may be slightly
 *              larger. If zero, the number of buckets will be roughly doubled.
 *
 * @return      A pointer to the hash table on success, NULL on failure.
 */
static void*
hashtab_grow(struct cgs_hashtab* ht, size_t size)
{
        // Get new size or return if smaller
        size_t new_size = 0;
        if (size == 0)
                new_size = cgs_next_prime(ht->size * 2);
        else if (size > ht->size)
                new_size = cgs_next_prime(size);
        else
                return NULL;

        // Allocate new table
        struct cgs_htab_bucket** ppb = malloc(new_size * HTAB_BUCKET_PSIZE);
        if (!ppb)
                return NULL;

        init_null_buckets(ppb, new_size);
        hashtab_rehash(ht, ppb, new_size);

        return ht;
}

/**
 * hashtab_pre_check_load
 *
 * Check if the hash table is able to add an element without going over max
 * load. Handles empty table.
 *
 * @param ht    The hash table.
 *
 * @return      A pointer to the hash table if there is room to add the
 *              element, NULL if there is not.
 */
static void*
hashtab_pre_check_load(struct cgs_hashtab* ht)
{
        // check if table is empty
        if (ht->size == 0)
                return hashtab_build(ht);

        // will adding an element overload the table?
        if ((double)(ht->length + 1) / (double)ht->size > ht->max_load)
                return hashtab_grow(ht, 0);
        return ht;
}

/**
 * hashtab_add_bucket
 *
 * Allocates a new bucket and adds it to the hash table.
 *
 * @param ht            The hash table.
 * @param key           The key to add.
 * @param hashval       The pre-calculated hash-value of the key.
 * @param value         A pointer to a variant containing the value. Optional,
 *                      may be NULL.
 *
 * @return              A pointer to the value member of the new bucket.
 */
static struct cgs_variant*
hashtab_add_bucket(struct cgs_hashtab* ht, const char* key, size_t hashval,
                const struct cgs_variant* value)
{
        // Ensure space available, re-calculate hashval if necessary
        size_t size = ht->size;
        if (!hashtab_pre_check_load(ht))
                return NULL;
        if (size != ht->size)
                hashval = ht->hash(key, ht->size);

        struct cgs_htab_bucket* b = cgs_htab_bucket_new(key);
        if (!b)
                return NULL;
        b->next = ht->table[hashval];
        ht->table[hashval] = b;
        ++ht->length;

        if (value)
                memcpy(&b->value, value, sizeof(*value));

        return &b->value;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_hashtab
cgs_hashtab_new(CgsFreeFunc ff)
{
        return (struct cgs_hashtab){
                .length = 0,
                .table = NULL,
                .hash = cgs_hash_str,
                .ff = ff,
                .cmp = cgs_str_cmp,
                .size = 0,
                .max_load = HTAB_DEFAULT_LOAD_FACTOR,
        };
}

void
cgs_hashtab_free(void* p)
{
        struct cgs_hashtab* ht = p;

        for (size_t i = 0; i < ht->size; ++i)
                cgs_htab_bucket_free(ht->table[i], ht->ff);

        free(ht->table);
}

void*
cgs_hashtab_reserve(struct cgs_hashtab* ht, size_t size)
{
        return hashtab_grow(ht, size);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Inline Function Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
size_t
cgs_hashtab_length(const struct cgs_hashtab* h);

double
cgs_hashtab_current_load(const struct cgs_hashtab* ht);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const void*
cgs_hashtab_lookup(const struct cgs_hashtab* ht, const char* key)
{
        if (ht->size == 0)      // empty hash table check
                return NULL;

        const struct cgs_htab_bucket* b = ht->table[ht->hash(key, ht->size)];
        while (b) {
                if (ht->cmp(&key, &b->key) == 0)
                        return cgs_variant_get(&b->value);
                b = b->next;
        }
        return NULL;
}

void*
cgs_hashtab_lookup_mut(struct cgs_hashtab* ht, const char* key)
{
        if (ht->size == 0)      // empty hash table check
                return NULL;

        struct cgs_htab_bucket* b = ht->table[ht->hash(key, ht->size)];
        while (b) {
                if (ht->cmp(&key, &b->key) == 0)
                        return cgs_variant_get_mut(&b->value);
                b = b->next;
        }
        return NULL;
}

struct cgs_variant*
cgs_hashtab_insert(struct cgs_hashtab* ht, const char* key,
                const struct cgs_variant* var)
{
        size_t hashval = ht->size == 0 ? 0 : ht->hash(key, ht->size);
        
        // Check for existing element
        if (ht->length > 0) {
                struct cgs_htab_bucket* p = ht->table[hashval];
                while (p) {
                        if (ht->cmp(&key, &p->key) == 0)
                                return NULL;
                        p = p->next;
                }
        }

        // No match in table, add new key-value pair
        return hashtab_add_bucket(ht, key, hashval, var);
}

struct cgs_variant*
cgs_hashtab_get(struct cgs_hashtab* ht, const char* key)
{
        size_t hashval = ht->size == 0 ? 0 : ht->hash(key, ht->size);

        // If key exists, return pointer to value
        if (ht->length > 0) {
                struct cgs_htab_bucket* p = ht->table[hashval];
                while (p) {
                        if (ht->cmp(&key, &p->key) == 0)
                                return &p->value;
                        p = p->next;
                }
        }

        // No match in table, add new key-value pair
        return hashtab_add_bucket(ht, key, hashval, NULL);
}

void
cgs_hashtab_remove(struct cgs_hashtab* h, const char* key)
{
        if (h->length == 0)
                return;

        size_t hashval = h->hash(key, h->size);
        struct cgs_htab_bucket* b = h->table[hashval];
        if (!b)
                return;

        struct cgs_htab_bucket* parent = NULL;
        while (b) {
                if (h->cmp(&b->key, &key) == 0)
                        break;
                parent = b;
                b = b->next;
        }

        if (parent)
                parent->next = b->next;
        else
                h->table[hashval] = b->next;

        --h->length;
        cgs_htab_bucket_free(b, h->ff);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Iterator
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_hashtab_iter_mut
cgs_hashtab_begin_mut(struct cgs_hashtab* ht)
{
        return (struct cgs_hashtab_iter_mut){
                .tab = ht->table,
                .cur = NULL,
                .end = &ht->table[ht->size],
        };
}

void*
cgs_hashtab_iter_mut_next(struct cgs_hashtab_iter_mut* it)
{
        // Ensure it->tab is pointing at an occupied bucket at all times
        while (!*(it->tab) && it->tab != it->end)
                ++it->tab;

        if (it->tab == it->end)
                return NULL;

        if (!it->cur) {
                it->cur = *(it->tab);
                return it;
        }

        // Walk current bucket list
        if (it->cur->next) {
                it->cur = it->cur->next;
                return it;
        }

        it->cur = NULL;
        ++it->tab;
        return cgs_hashtab_iter_mut_next(it);
}

struct cgs_variant*
cgs_hashtab_iter_mut_get(struct cgs_hashtab_iter_mut* it)
{
        return &it->cur->value;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum hash_function_constants {
        STRING_HASH_MULTIPLIER = 37,
};

size_t
cgs_hash_str(const void* key, size_t size)
{
        size_t h = 0;
        for (const char* s = key; *s; ++s)
                h = STRING_HASH_MULTIPLIER * h + *s;
        return h % size;
}
