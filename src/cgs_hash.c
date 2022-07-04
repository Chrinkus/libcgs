#include "cgs_hash.h"

#include <stdlib.h>

enum { CGS_HASH_INIT = 31, CGS_HASH_MULT = 37, };

void*
cgs_hash_new(struct cgs_hash* h, CgsHashFunc f)
{
        struct cgs_list** p = malloc(CGS_HASH_INIT * sizeof(struct cgs_list*));
        if (!p)
                return NULL;

        for (int i = 0; i < CGS_HASH_INIT; ++i)
                p[i] = NULL;

        h->length = 0;
        h->size = CGS_HASH_INIT;
        h->hash = f;
        h->table = p;

        return h;
}

void
cgs_hash_free(struct cgs_hash* h)
{
        if (!h)
                return;
        for (size_t i = 0; i < h->size; ++i)
                while (h->table[i]) {
                        struct cgs_list* p = h->table[i];
                        h->table[i] = p->next;
                        free(p);
                }
        free(h->table);
}

size_t
cgs_hash_func_string(const struct cgs_variant* var, size_t size)
{
        size_t h = 0;
        for (const char* s = cgs_variant_get(var); *s; ++s)
                h = CGS_HASH_MULT * h + *s;
        return h % size;
}

