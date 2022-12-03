/* cgs_compare.c
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
#include "cgs_compare.h"

#include <string.h>

int cgs_int_cmp(const void* a, const void* b)
{
	int v1 = *(const int*)a;
	int v2 = *(const int*)b;

	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}

int cgs_int_cmp_rev(const void* a, const void* b)
{
        int v1 = *(const int*)a;
        int v2 = *(const int*)b;

        if (v1 > v2)
                return -1;
        else if (v1 < v2)
                return 1;
        else
                return 0;
}

int cgs_str_cmp(const void* a, const void* b)
{
	const char* s1 = *(const char**)a;
	const char* s2 = *(const char**)b;

	return strcmp(s1, s2);
}

int cgs_char_cmp(const void* a, const void* b)
{
	return *(char*)a - *(char*)b;
}

int cgs_int_pred(const void* a, const void* b)
{
        int v1 = *(const int*)a;
        int v2 = *(const int*)b;

        return v1 == v2;
}

