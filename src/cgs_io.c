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

#include <stdlib.h>

int
cgs_io_getline(FILE* file, struct cgs_string* buff)
{
	int count = 0;

	for (int c; (c = fgetc(file)) != EOF && c != '\n'; ++count)
                if (!cgs_string_push(buff, c))
                        return -1;

	return count;
}

char*
cgs_io_readline(FILE* file)
{
        struct cgs_string buffer = cgs_string_new();

	if (cgs_io_getline(file, &buffer) > 0)
                return cgs_string_xfer(&buffer);

        cgs_string_free(&buffer);

	return NULL;
}

void*
cgs_io_readlines(FILE* file, struct cgs_vector* lines)
{
        struct cgs_string buffer = cgs_string_new();

        while (cgs_io_getline(file, &buffer) > 0) {
                struct cgs_string line = cgs_string_new();
                if (!cgs_string_copy(&buffer, &line))
                        goto error_cleanup;
                if (!cgs_vector_push(lines, &line))
                        goto error_cleanup;

                cgs_string_clear(&buffer);
        }
	cgs_string_free(&buffer);
	return lines;

error_cleanup:
        cgs_string_free(&buffer);
        return NULL;
}

void*
cgs_io_readfile(const char* fname, struct cgs_string* buff)
{
        FILE* file = fopen(fname, "r");
        if (!file)
                return NULL;

        for (int c; (c = fgetc(file)) != EOF; )
                if (!cgs_string_push(buff, c))
                        goto error_cleanup;

        fclose(file);
        return buff;

error_cleanup:
        fclose(file);
        return NULL;
}
