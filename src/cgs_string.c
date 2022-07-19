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
#include "cgs_string_utils.h"
#include "cgs_compare.h"
#include "cgs_defs.h"

#include <stdlib.h>
#include <string.h>

enum cgs_string_defaults {
        CGS_STRING_INITIAL_CAPACITY = 16,
        CGS_STRING_GROWTH_RATE = 2,
};

void*
cgs_string_new(struct cgs_string* s)
{
        char* p = malloc(sizeof(char) * CGS_STRING_INITIAL_CAPACITY);
        if (!p)
                return NULL;

        p[0] = '\0';
        s->length = 0;
        s->capacity = CGS_STRING_INITIAL_CAPACITY;
        s->data = p;

        return s;
}

void*
cgs_string_new_from_str(struct cgs_string* s, const char* src)
{
        int len = strlen(src);
        char* p = malloc(len + 1);
        if (!p)
                return NULL;

        strcpy(p, src);
        s->length = len;
        s->capacity = len + 1;
        s->data = p;

	return s;
}

void
cgs_string_free(struct cgs_string* s)
{
	if (s)
		free(s->data);
}

void*
cgs_string_shrink(struct cgs_string* s)
{
        char* p = realloc(s->data, s->length + 1);
        if (!p)
                return NULL;

        s->data = p;
        s->capacity = s->length + 1;
        return s;
}

char*
cgs_string_xfer(struct cgs_string* s)
{
	char* p = s->data;
	s->data = NULL;
        s->capacity = 0;
	return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Inline Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const char*
cgs_string_data(const struct cgs_string* s);

char*
cgs_string_data_mutable(struct cgs_string* s);

size_t
cgs_string_length(const struct cgs_string* s);

const char*
cgs_string_get(const struct cgs_string* s, size_t i);

char*
cgs_string_get_mutable(struct cgs_string* s, size_t i);

char
cgs_string_char(const struct cgs_string* s, size_t i);

const char*
cgs_string_end(const struct cgs_string* s);

char*
cgs_string_end_mutable(struct cgs_string* s);

const char*
cgs_string_begin(const struct cgs_string* s);

char*
cgs_string_begin_mutable(struct cgs_string* s);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Static Helper Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
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
static size_t
cgs_string_new_capacity(size_t old)
{
        return old > 0 ? old * CGS_STRING_GROWTH_RATE
                       : CGS_STRING_INITIAL_CAPACITY;
}

static const char*
cgs_string_grow(struct cgs_string* s)
{
	size_t new_cap = cgs_string_new_capacity(s->capacity);

	char* p = realloc(s->data, new_cap);
        if (!p)
                return NULL;

        s->data = p;
        s->capacity = new_cap;
	return p;
}

static const char*
cgs_string_grow_len(struct cgs_string* s, size_t len)
{
        size_t new_cap = cgs_string_new_capacity(s->capacity);
        new_cap = CGS_MAX(len + 1, new_cap);

        char* p = realloc(s->data, new_cap);
        if (!p)
                return NULL;

        s->data = p;
        s->capacity = new_cap;
        return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

const char*
cgs_string_push(struct cgs_string* s, int c)
{
	if (s->length == s->capacity - 1 && !cgs_string_grow(s))
		return NULL;
	s->data[s->length++] = c;
	s->data[s->length] = '\0';
	return s->data;
}

struct cgs_string*
cgs_string_prepend_str(struct cgs_string* s, const char* add, size_t len)
{
        size_t new_len = s->length + len;
        if (new_len >= s->capacity - 1 && !cgs_string_grow_len(s, new_len))
                return NULL;

        cgs_strprepend(s->data, add, len);

        s->length = new_len;
        return s;
}

struct cgs_string*
cgs_string_append_str(struct cgs_string* s, const char* add, size_t len)
{
        size_t new_len = s->length + len;
        if (new_len >= s->capacity - 1 && !cgs_string_grow_len(s, new_len))
                return NULL;

        strncat(s->data, add, len);

        s->length = new_len;
        return s;
}

struct cgs_string*
cgs_string_prepend(struct cgs_string* s, const struct cgs_string* add)
{
        return cgs_string_prepend_str(s, add->data, add->length);
}

struct cgs_string*
cgs_string_append(struct cgs_string* s, const struct cgs_string* add)
{
        return cgs_string_append_str(s, add->data, add->length);
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

