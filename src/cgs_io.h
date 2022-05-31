/* cgs_io.h
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
#pragma once

#include <stdio.h>
/* The cgs_string and cgs_array headers are included rather than just forward
 * declaring the structs since usage of the io functions demands their
 * inclusion.
 */
#include "cgs_string.h"
#include "cgs_array.h"

/**
 * cgs_io_getline
 *
 * Read a line from file and store it in buff. Reading terminates on EOF or
 * newline character. The newline is discarded.
 *
 * @param file	The file or stream to read from.
 * @param buff	The self-managed string buffer.
 *
 * @return	The number of characters read.
 */
int
cgs_io_getline(FILE* file, struct cgs_string* buff);

/**
 * cgs_io_readline
 *
 * Read a line from a file and return a pointer to it.
 *
 * @param file	The file or stream to read from.
 *
 * @return	An allocated null-terminated string.
 */
char*
cgs_io_readline(FILE* file);

/**
 * cgs_io_readlines
 *
 * Read all lines from a file and store them in a cgs_array sized for char*'s.
 * 
 * @param file	The file or stream to read from.
 *
 * @return	A cgs_array of char*'s.
 */
struct cgs_array
cgs_io_readlines(FILE* file);

