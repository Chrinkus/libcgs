/* cgs_string.h
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
#include "cgs_string.h"
#include "cgs_string_private.h"
#include "cgs_string_utils.h"
#include "cgs_compare.h"
#include "cgs_defs.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * CGS_EMPTY_STRING
 *
 * An empty string for non-allocated string structs to use.
 */
char* const CGS_EMPTY_STRING = "";

struct cgs_string
cgs_string_new(void)
{
        return (struct cgs_string){
                .length = 0,
                .capacity = 0,
                .data = CGS_EMPTY_STRING,
        };
}

void*
cgs_string_copy(const void* s, void* d)
{
        const struct cgs_string* src = s;
        struct cgs_string* dst = d;

        const size_t new_cap = src->length + 1;
        if (dst->capacity < src->capacity && !cgs_string_alloc(dst, new_cap))
                return NULL;

        strcpy(dst->data, src->data);
        dst->length = src->length;
        dst->capacity = new_cap;
        return dst;
}

void
cgs_string_move(void* s, void* d)
{
        struct cgs_string* src = s;
        struct cgs_string* dst = d;

        dst->length = src->length;
        dst->capacity = src->capacity;
        dst->data = src->data;

        src->length = 0;
        src->capacity = 0;
        src->data = CGS_EMPTY_STRING;
}

void*
cgs_string_from(const char* src, struct cgs_string* s)
{
        size_t len = strlen(src);
        if (!cgs_string_alloc(s, len + 1))
                return NULL;

        strcpy(s->data, src);
        s->length = len;
	return s;
}

void*
cgs_string_from_int(int n, struct cgs_string* s)
{
        int sign = n < 0 ? -1 : 1;
        n *= sign;

        do {
                if (!cgs_string_push(s, '0' + n % 10))
                        return NULL;
        } while ((n /= 10) != 0);

        if (sign == -1 && !cgs_string_push(s, '-'))
                return NULL;

        cgs_string_reverse(s);
        return s;
}

void
cgs_string_free(void* p)
{
        struct cgs_string* s = p;
	if (s && s->data != CGS_EMPTY_STRING)
		free(s->data);
}

void*
cgs_string_shrink(struct cgs_string* s)
{
        if (!cgs_string_alloc(s, s->length + 1))
                return NULL;
        return s;
}

char*
cgs_string_xfer(struct cgs_string* s)
{
	char* p = s->data;
	s->data = CGS_EMPTY_STRING;
        s->capacity = 0;
	return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Inline Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const char*
cgs_string_data(const struct cgs_string* s);

char*
cgs_string_data_mut(struct cgs_string* s);

size_t
cgs_string_length(const struct cgs_string* s);

const char*
cgs_string_get(const struct cgs_string* s, size_t i);

char*
cgs_string_get_mut(struct cgs_string* s, size_t i);

char
cgs_string_char(const struct cgs_string* s, size_t i);

const char*
cgs_string_end(const struct cgs_string* s);

char*
cgs_string_end_mut(struct cgs_string* s);

const char*
cgs_string_begin(const struct cgs_string* s);

char*
cgs_string_begin_mut(struct cgs_string* s);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Static Helper Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 */
void*
cgs_string_alloc(struct cgs_string* s, size_t cap)
{
        char* p = NULL;
        if (s->data == CGS_EMPTY_STRING)
                p = malloc(cap);
        else
                p = realloc(s->data, cap);

        if (!p)
                return NULL;

        if (s->data == CGS_EMPTY_STRING)
                p[0] = '\0';

        s->data = p;
        s->capacity = cap;
        return s;
}

/**
 * cgs_string_new_capacity
 *
 * Calculate an appropriate new capacity for a string based off of the current
 * capacity.
 *
 * @param old   The old capacity of a string.
 *
 * @return      A new capacity based off the old.
 */
size_t
cgs_string_new_capacity(size_t old)
{
        return old > 0 ? old * CGS_STRING_GROWTH_RATE
                       : CGS_STRING_INITIAL_CAPACITY;
}

void*
cgs_string_grow(struct cgs_string* s)
{
	size_t new_cap = cgs_string_new_capacity(s->capacity);

        if (!cgs_string_alloc(s, new_cap))
                return NULL;

	return s;
}

void*
cgs_string_grow_len(struct cgs_string* s, size_t len)
{
        size_t new_cap = cgs_string_new_capacity(s->capacity);
        new_cap = CGS_MAX(len + 1, new_cap);

        if (!cgs_string_alloc(s, new_cap))
                return NULL;
        return s;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int
cgs_string_cmp(const void* a, const void* b)
{
        const struct cgs_string* s1 = a;
        const struct cgs_string* s2 = b;

        return strcmp(cgs_string_data(s1), cgs_string_data(s2));
}

int
cgs_string_eq_str(const void* a, const void* b)
{
        const struct cgs_string* s1 = a;
        const char* s2 = b;

        return strcmp(cgs_string_data(s1), s2) == 0;
}

void*
cgs_string_push(struct cgs_string* s, int c)
{
        if (s->capacity == 0 || s->length == s->capacity - 1) {
                if (!cgs_string_grow(s))
                        return NULL;
        }

	s->data[s->length++] = c;
	s->data[s->length] = '\0';
	return s;
}

void*
cgs_string_cat(struct cgs_string* dst, const struct cgs_string* src)
{
        size_t new_cap = dst->length + src->length + 1;
        if (dst->capacity < new_cap && !cgs_string_grow_len(dst, new_cap))
                return NULL;
        strcat(dst->data, src->data);
        dst->length = new_cap - 1;
        return dst;
}

void
cgs_string_clear(struct cgs_string* s)
{
	s->data[0] = '\0';
	s->length = 0;
}

void
cgs_string_erase(struct cgs_string* s)
{
	for (char* p = s->data; p < s->data + s->capacity; ++p)
		*p = '\0';
	s->length = 0;
}

void
cgs_string_sort(struct cgs_string* s)
{
	qsort(s->data, s->length, sizeof(char), cgs_char_cmp);
}

void
cgs_string_trunc(struct cgs_string* s, size_t n)
{
        if (s->length <= n)
                return;
        s->data[n] = '\0';
        s->length = n;
}

void
cgs_string_reverse(struct cgs_string* s)
{
        for (char* b = s->data, *e = &s->data[s->length-1]; b < e; ++b, --e)
                CGS_SWAP(*b, *e, char);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Strsub Inline Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_strsub
cgs_strsub_new(const char* s, size_t len);

struct cgs_strsub
cgs_strsub_from_str(const char* s);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Strsub Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int
cgs_strsub_cmp(const void* a, const void* b)
{
        const struct cgs_strsub* ss1 = a;
        const struct cgs_strsub* ss2 = b;

        // same length --> memcmp
        if (ss1->length == ss2->length) {
                return memcmp(ss1->data, ss2->data, ss1->length);
        }

        // diff lengths --> memcmp up to shortest length,
        // shorter strsub is less if both are equal.
        if (ss1->length < ss2->length) {
                return memcmp(ss1->data, ss2->data, ss1->length) <= 0 ? -1 : 1;
        } else {
                return memcmp(ss1->data, ss2->data, ss2->length) >= 0 ? 1 : -1;
        }
}

int
cgs_strsub_eq_str(const struct cgs_strsub* ss, const char* s)
{
        return strlen(s) == ss->length
                && strncmp(ss->data, s, ss->length) == 0;
}

void*
cgs_strsub_to_int(const struct cgs_strsub* ss, int* out)
{
        const char* s = ss->data;
        size_t i = 0;
        while (i < ss->length && isspace(s[i]))
                ++i;

        int sign = 1;
        if (s[i] == '-' || s[i] == '+')
                sign = s[i++] == '-' ? -1 : 1;

        if (i >= ss->length || !isdigit(s[i]))
                return NULL;

        *out = 0;
        for ( ; i < ss->length && isdigit(ss->data[i]); ++i) {
                *out *= 10;
                *out += ss->data[i] - '0';
        }

        *out *= sign;
        return out;
}

char*
cgs_strsub_to_str(const struct cgs_strsub* ss)
{
        char* p = malloc(ss->length + 1);
        if (!p)
                return NULL;

        strncpy(p, ss->data, ss->length);
        p[ss->length] = '\0';
        return p;
}

void*
cgs_strsub_to_string(const struct cgs_strsub* ss, struct cgs_string* dst)
{
        char* p = cgs_strsub_to_str(ss);
        if (!p)
                return NULL;

        dst->data = p;
        dst->length = ss->length;
        dst->capacity = ss->length + 1;

        return dst;
}

void*
cgs_str_split(const char* s, char delim, struct cgs_vector* vec)
{
        if (vec->element_size != sizeof(struct cgs_strsub))
                return NULL;

        for (const char* p = s; ; ++s, p = s) {
                size_t count = 0;
                while (*s && *s != delim) {
                        ++s;
                        ++count;
                }
                if (count != 0) {
                        struct cgs_strsub ss = cgs_strsub_new(p, count);
                        if (!cgs_vector_push(vec, &ss))
                                return NULL;
                }
                if (!*s)
                        break;
        }
        return vec;
}

// Inline symbol
void*
cgs_string_split(const struct cgs_string* s, char delim, struct cgs_vector* v);

void*
cgs_strsub_split(const struct cgs_strsub* ss, char delim,
                struct cgs_vector* vec)
{
        if (vec->element_size != sizeof(struct cgs_strsub))
                return NULL;

        const char* beg = ss->data;
        const char* end = beg + ss->length;
        for ( ; beg < end; ++beg) {
                const char* start = beg;
                size_t count = 0;
                while (beg < end && *beg != delim) {
                        ++beg;
                        ++count;
                }
                if (count != 0) {
                        struct cgs_strsub ss = cgs_strsub_new(start, count);
                        if (!cgs_vector_push(vec, &ss))
                                return NULL;
                }
        }
        return vec;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * C-String Utility Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void*
cgs_string_prepend_str(struct cgs_string* s, const char* add, size_t len)
{
        size_t new_len = s->length + len;
        if (new_len >= s->capacity - 1 && !cgs_string_grow_len(s, new_len))
                return NULL;

        cgs_strprepend(s->data, add, len);

        s->length = new_len;
        return s;
}

void*
cgs_string_cat_str(struct cgs_string* s, const char* add, size_t len)
{
        size_t new_len = s->length + len;
        if (new_len >= s->capacity - 1 && !cgs_string_grow_len(s, new_len))
                return NULL;

        strncat(s->data, add, len);

        s->length = new_len;
        return s;
}

