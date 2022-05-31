/* cgs_string_utils.c
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
#include "cgs_string_utils.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* cgs_strdup(const char* src)
{
	int len = strlen(src) + 1;
	char* dst = malloc(len * sizeof(src[0]));

	if (dst)
		strcpy(dst, src);
	return dst;
}

void cgs_strappend(char* s, char ch)
{
	while (*s)
		++s;
	*s++ = ch;
	*s = '\0';
}

void cgs_strprepend(char* s, char ch)
{
	for (char tmp = *s; ch; *s++ = ch, ch = tmp)
		if (tmp)
			tmp = *s;
	*s = '\0';
}

void cgs_strshift(char* s, size_t n)
{
	if (strlen(s) < n) {
		*s = '\0';
	} else {
		for (char* p = s+n; *s; ++s, ++p)
			*s = *p;
	}
}

void cgs_strtrim(char* s)
{
	char* p = s;
	while (isspace(*p))
		++p;

	while (*p)
		*s++ = *p++;

	do {
		*s = '\0';
	} while (isspace(*--s));
}

void cgs_strtrimch(char* s, char ch)
{
	char* p = s;
	while (*p == ch)
		++p;

	while (*p)
		*s++ = *p++;

	do {
		*s = '\0';
	} while (*--s == ch);
}

