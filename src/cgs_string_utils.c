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
	char* dst = malloc(strlen(src) + 1);
        if (!dst)
                return NULL;

        strcpy(dst, src);

	return dst;
}

void cgs_strmove(char* s, size_t n)
{
        char* p = s + n;
        memmove(p, s, strlen(s) + 1);
}

void cgs_strprepend(char* dst, const char* src, size_t n)
{
        cgs_strmove(dst, n);
        for (size_t i = 0; i < n; ++i)
                dst[i] = src[i];
}

void cgs_strappendch(char* s, char ch)
{
	while (*s)
		++s;
	*s++ = ch;
	*s = '\0';
}

void cgs_strprependch(char* s, char ch)
{
	for (char tmp = *s; ch; *s++ = ch, ch = tmp)
		if (tmp)
			tmp = *s;
	*s = '\0';
}

void cgs_strshiftl(char* s, size_t n, int c)
{
        const size_t len = strlen(s);
        if (len < n) {
                for (size_t i = 0; i < len; ++i)
                        s[i] = c;
        } else {
                size_t i = 0;
                while (s[n])
                        s[i++] = s[n++];
                while (i < len)
                        s[i++] = c;
        }
}

void cgs_strshiftr(char* s, size_t n, int c)
{
        const size_t len = strlen(s);
        if (len < n) {
                for (size_t i = 0; i < len; ++i)
                        s[i] = c;
        } else {
                size_t i = len - 1;
                for (size_t j = i - n; j < i; --j, --i)
                        s[i] = s[j];
                while (i < len)
                        s[i--] = c;
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

