/* cgs_error.c
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
#include "cgs_error.h"
#include "cgs_defs.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

enum { ERROR_BUFFMIN = 256, ERROR_BUFFMAX = 512 };

static void
cgs_error_print(const char* format, va_list ap)
{
        char buf[ERROR_BUFFMAX];
        char msg[ERROR_BUFFMIN];

        vsnprintf(msg, ERROR_BUFFMIN, format, ap);

        snprintf(buf, ERROR_BUFFMAX, "LIBCGS ERROR: %s\n", msg);

        fflush(stdout);
        fputs(buf, stderr);
        fflush(stderr);
}

int
cgs_error_retfail(const char* format, ...)
{
        va_list ap;

        va_start(ap, format);
        cgs_error_print(format, ap);
        va_end(ap);

        return EXIT_FAILURE;
}

void*
cgs_error_retnull(const char* format, ...)
{
        va_list ap;

        va_start(ap, format);
        cgs_error_print(format, ap);
        va_end(ap);

        return NULL;
}

int
cgs_error_retbool(const char* format, ...)
{
        va_list ap;

        va_start(ap, format);
        cgs_error_print(format, ap);
        va_end(ap);

        return CGS_FALSE;
}

char*
cgs_error_sys(void)
{
        return strerror(errno);
}

