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

#include <stdlib.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum hash_function_magic {
        STRING_HASH_MULTIPLIER = 37,
};

size_t
cgs_string_hash(const void* key, size_t size)
{
        size_t h = 0;
        for (const char* s = key; *s; ++s)
                h = STRING_HASH_MULTIPLIER * h + *s;
        return h % size;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Private Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
enum hash_table_magic {
        HASHTAB_NUM_BUCKETS = 31,
        HASHTAB_BUCKET_PSIZE = sizeof(struct cgs_bucket*),
        HASHTAB_INITIAL_ALLOC = HASHTAB_NUM_BUCKETS * HASHTAB_BUCKET_PSIZE,
};

const double HASHTAB_DEFAULT_LOAD_FACTOR = 0.8;

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Bucket Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_bucket_new
 *
 * Allocates a new bucket and key-string.
 * 
 * @param key   The string to use as a key.
 *
 * @return      A pointer to the newly allocated bucket on success or NULL
 *              on failure.
 */
static struct cgs_bucket*
cgs_bucket_new(const char* key)
{
        struct cgs_bucket* b = malloc(sizeof(struct cgs_bucket));
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
 * cgs_bucket_free
 *
 * Frees the memory allocated to a bucket, its key and any memory allocated
 * to the variant value.
 *
 * @param b     The bucket to free.
 */
static void
cgs_bucket_free(struct cgs_bucket* b)
{
        cgs_variant_free_data(&b->value);
        free(b->key);
        free(b);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void*
cgs_hashtab_new(struct cgs_hashtab* tab)
{
        struct cgs_bucket** ppb = malloc(HASHTAB_INITIAL_ALLOC);
        if (!ppb)
                return NULL;

        for (int i = 0; i < HASHTAB_NUM_BUCKETS; ++i)
                ppb[i] = NULL;

        tab->length = 0;
        tab->table = ppb;
        tab->hash = cgs_string_hash;
        tab->cmp = cgs_str_cmp;
        tab->size = HASHTAB_NUM_BUCKETS;
        tab->max_load = HASHTAB_DEFAULT_LOAD_FACTOR;

        return tab;
}

void
cgs_hashtab_free(struct cgs_hashtab* tab)
{
        if (!tab)
                return;
        for (size_t i = 0; i < tab->size; ++i)
                while (tab->table[i]) {
                        struct cgs_bucket* b = tab->table[i];
                        tab->table[i] = b->next;
                        cgs_bucket_free(b);
                }
        free(tab->table);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Inline Function Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
size_t
cgs_hashtab_length(const struct cgs_hashtab* h);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const void*
cgs_hashtab_lookup(const struct cgs_hashtab* h, const char* key)
{
        size_t hashval = h->hash(key, h->size);
        for (const struct cgs_bucket* b = h->table[hashval]; b; b = b->next)
                if (h->cmp(&b->key, &key) == 0)                 // &!!
                        return cgs_variant_get(&b->value);

        return NULL;
}

struct cgs_variant*
cgs_hashtab_get(struct cgs_hashtab* h, const char* key)
{
        size_t hashval = h->hash(key, h->size);

        struct cgs_bucket* p = h->table[hashval];
        if (p) {
                for (struct cgs_bucket* tmp = p; tmp; tmp = tmp->next)
                        if (h->cmp(&tmp->key, &key) == 0)       // &!!
                                return &tmp->value;
        }

        struct cgs_bucket* b = cgs_bucket_new(key);
        if (!b)
                return NULL;
        b->next = p;            // works if p is NULL or a list!
        h->table[hashval] = b;
        ++h->length;
        return &b->value;
}

void
cgs_hashtab_remove(struct cgs_hashtab* h, const char* key)
{
        size_t hashval = h->hash(key, h->size);
        struct cgs_bucket* b = h->table[hashval];
        if (!b)
                return;

        struct cgs_bucket* parent = NULL;
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
        cgs_bucket_free(b);
}

