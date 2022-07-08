/* cgs_hash.c
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
#include "cgs_hash.h"
#include "cgs_compare.h"
#include "cgs_string_utils.h"

#include <stdlib.h>

enum {
        CGS_HASH_MULTIPLIER = 37,
        CGS_HASH_NUM_BUCKETS = 31,
        CGS_HASH_BUCKET_PSIZE = sizeof(struct cgs_bucket*),
        CGS_HASH_INITIAL_ALLOC = CGS_HASH_NUM_BUCKETS * CGS_HASH_BUCKET_PSIZE,
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void*
cgs_hash_new(struct cgs_hash* tab)
{
        struct cgs_bucket** ppb = malloc(CGS_HASH_INITIAL_ALLOC);
        if (!ppb)
                return NULL;

        for (int i = 0; i < CGS_HASH_NUM_BUCKETS; ++i)
                ppb[i] = NULL;

        tab->length = 0;
        tab->size = CGS_HASH_NUM_BUCKETS;
        tab->hash = cgs_string_hash;
        tab->cmp = cgs_str_cmp;
        tab->table = ppb;

        return tab;
}

void
cgs_hash_free(struct cgs_hash* tab)
{
        if (!tab)
                return;
        for (size_t i = 0; i < tab->size; ++i)
                while (tab->table[i]) {
                        struct cgs_bucket* b = tab->table[i];
                        tab->table[i] = b->next;
                        cgs_variant_free_data(&b->value);
                        free(b->key);
                        free(b);
                }
        free(tab->table);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Bucket Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
size_t
cgs_hash_length(const struct cgs_hash* h);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
size_t
cgs_string_hash(const void* key)
{
        size_t h = 0;
        for (const char* s = key; *s; ++s)
                h = CGS_HASH_MULTIPLIER * h + *s;
        return h;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hash Table Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const void*
cgs_hash_lookup(const struct cgs_hash* h, const char* key)
{
        size_t hashval = h->hash(key) % h->size;
        for (const struct cgs_bucket* b = h->table[hashval]; b; b = b->next)
                if (h->cmp(&b->key, &key) == 0)                 // &!!
                        return cgs_variant_get(&b->value);

        return NULL;
}

struct cgs_variant*
cgs_hash_get(struct cgs_hash* h, const char* key)
{
        size_t hashval = h->hash(key) % h->size;

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

