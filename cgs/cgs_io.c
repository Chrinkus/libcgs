/* cgs_io.c
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
#include "cgs_io.h"
#include "cgs_string_utils.h"

int
cgs_io_getline(FILE* file, struct cgs_string* buff)
{
	int count = 0;

	for (int c; (c = fgetc(file)) != EOF && c != '\n'; ++count)
		cgs_string_push(buff, c);

	return count;
}

struct cgs_array*
cgs_io_readlines(FILE* file)
{
	struct cgs_array* lines = cgs_array_new(char*);
	struct cgs_string* buffer = cgs_string_new();

	for ( ; cgs_io_getline(file, buffer) > 0; cgs_string_clear(buffer)) {
		char* line = cgs_strdup(cgs_string_read(buffer));
		cgs_array_push(lines, &line);
	}
	cgs_string_free(buffer);

	return lines;
}

