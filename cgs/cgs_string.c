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
#include "cgs_compare.h"

#include <stdlib.h>
#include <string.h>

enum cgs_string_defaults {
	CGS_STRING_INITIAL_CAPACITY = 16,
	CGS_STRING_GROWTH_RATE = 2,
};

struct cgs_string {
	size_t length;
	size_t capacity;
	char* memory;
};

struct cgs_string*
cgs_string_new(void)
{
	struct cgs_string* s = malloc(sizeof(struct cgs_string));
	if (!s)
		return NULL;

	s->capacity = CGS_STRING_INITIAL_CAPACITY;
	s->memory = malloc(sizeof(char) * s->capacity);
	if (!s->memory) {
		free(s);
		return NULL;
	}

	s->length = 0;
	s->memory[0] = '\0';
	return s;
}

struct cgs_string*
cgs_string_new_from_string(const char* src)
{
	struct cgs_string* s = malloc(sizeof(struct cgs_string));
	if (!s)
		return NULL;

	s->length = strlen(src);
	s->capacity = s->length + 1;
	s->memory = malloc(sizeof(char) * s->capacity);
	if (!s->memory) {
		free(s);
		return NULL;
	}
	strcpy(s->memory, src);
	return s;
}

void
cgs_string_free(struct cgs_string* s)
{
	if (s) {
		free(s->memory);
		free(s);
	}
}

char*
cgs_string_xfer(struct cgs_string* s)
{
	char* p = s->memory;
	s->memory = NULL;
	free(s);
	return p;
}

const char*
cgs_string_read(const struct cgs_string* s)
{
	return s->memory;
}

size_t
cgs_string_length(const struct cgs_string* s)
{
	return s->length;
}

static const char*
cgs_string_grow(struct cgs_string* s)
{
	size_t new_cap = s->capacity > 0
		? s->capacity * CGS_STRING_GROWTH_RATE
		: CGS_STRING_INITIAL_CAPACITY;

	char* p = realloc(s->memory, new_cap);
	if (p) {
		s->memory = p;
		s->capacity = new_cap;
	}
	return p;
}

const char*
cgs_string_push(struct cgs_string* s, int c)
{
	if (s->length == s->capacity - 1 && !cgs_string_grow(s))
		return NULL;
	s->memory[s->length++] = c;
	s->memory[s->length] = '\0';
	return s->memory;
}

void
cgs_string_clear(struct cgs_string* s)
{
	s->memory[0] = '\0';
	s->length = 0;
}

void
cgs_string_erase(struct cgs_string* s)
{
	for (char* p = s->memory; p < s->memory + s->capacity; ++p)
		*p = '\0';
	s->length = 0;
}

void
cgs_string_sort(struct cgs_string* s)
{
	qsort(s->memory, s->length, sizeof(char), cgs_char_cmp);
}

