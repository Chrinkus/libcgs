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
cgs_htab_bucket_free(void* p)
{
        if (!p)
                return;

        struct cgs_htab_bucket* b = p;

        //cgs_htab_bucket_free(b->next);
        cgs_variant_free_data(&b->value);
        free(b->key);
        free(b);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Private Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
static void
hashtab_check_load_factor(struct cgs_hashtab* ht)
{
        if (cgs_hashtab_current_load(ht) > ht->max_load)
                cgs_hashtab_rehash(ht, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void*
cgs_hashtab_new(struct cgs_hashtab* tab)
{
        struct cgs_htab_bucket** ppb = malloc(HTAB_INITIAL_ALLOC);
        if (!ppb)
                return NULL;

        for (int i = 0; i < HTAB_DEFAULT_SIZE; ++i)
                ppb[i] = NULL;

        tab->length = 0;
        tab->table = ppb;
        tab->hash = cgs_hash_str;
        tab->cmp = cgs_str_cmp;
        tab->size = HTAB_DEFAULT_SIZE;
        tab->max_load = HTAB_DEFAULT_LOAD_FACTOR;

        return tab;
}

void
cgs_hashtab_free(void* p)
{
        struct cgs_hashtab* tab = p;

        for (size_t i = 0; i < tab->size; ++i)
                while (tab->table[i]) {
                        struct cgs_htab_bucket* b = tab->table[i];
                        tab->table[i] = b->next;
                        cgs_htab_bucket_free(b);
                }
        free(tab->table);
}

void*
cgs_hashtab_rehash(struct cgs_hashtab* ht, size_t size)
{
        // Get new size or return if smaller
        size_t new_size = 0;
        if (size == 0)
                new_size = cgs_next_prime(ht->size * 2);
        else if (size > ht->size)
                new_size = size;
        else
                return NULL;

        // Allocate new table and set all buckets to NULL
        struct cgs_htab_bucket** new_tab = malloc(new_size * HTAB_BUCKET_PSIZE);
        if (!new_tab)
                return NULL;

        for (size_t i = 0; i < new_size; ++i)
                new_tab[i] = NULL;

        // Rehash table
        for (size_t i = 0; i < ht->size; ++i) {
                struct cgs_htab_bucket* bp = ht->table[i];
                while (bp) {
                        struct cgs_htab_bucket* tmp = bp->next;
                        size_t hash = ht->hash(bp->key, new_size);
                        bp->next = new_tab[hash];
                        new_tab[hash] = bp;
                        bp = tmp;
                }
        }

        free(ht->table);
        ht->table = new_tab;
        ht->size = new_size;

        return ht;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Inline Function Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
size_t
cgs_hashtab_length(const struct cgs_hashtab* h);

double
cgs_hashtab_current_load(const struct cgs_hashtab* h);

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
 * Hash Table Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const void*
cgs_hashtab_lookup(const struct cgs_hashtab* h, const char* key)
{
        size_t hashval = h->hash(key, h->size);
        for (const struct cgs_htab_bucket* b = h->table[hashval]; b; b = b->next)
                if (h->cmp(&b->key, &key) == 0)                 // &!!
                        return cgs_variant_get(&b->value);

        return NULL;
}

struct cgs_variant*
cgs_hashtab_get(struct cgs_hashtab* h, const char* key)
{
        size_t hashval = h->hash(key, h->size);

        struct cgs_htab_bucket* p = h->table[hashval];
        if (p) {
                for (struct cgs_htab_bucket* tmp = p; tmp; tmp = tmp->next)
                        if (h->cmp(&tmp->key, &key) == 0)       // &!!
                                return &tmp->value;
        }

        struct cgs_htab_bucket* b = cgs_htab_bucket_new(key);
        if (!b)
                return NULL;
        b->next = p;            // works if p is NULL or a list!
        h->table[hashval] = b;
        ++h->length;

        hashtab_check_load_factor(h);

        return &b->value;
}

void
cgs_hashtab_remove(struct cgs_hashtab* h, const char* key)
{
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
        cgs_htab_bucket_free(b);
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
