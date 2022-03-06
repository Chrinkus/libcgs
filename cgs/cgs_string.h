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
#pragma once

#include <stddef.h>	/* size_t */

struct cgs_string;

/**
 * cgs_string_new
 *
 * Allocate a new string and return it. Size will be 0 and the first character
 * will be '\0'.
 *
 * @return	The newly allocated string.
 */
struct cgs_string*
cgs_string_new(void);

/**
 * cgs_string_new_from_string
 *
 * Create a new string by copying the contents of another string.
 *
 * @param src	Source string to copy from.
 *
 * @return	The newly allocated string.
 */
struct cgs_string*
cgs_string_new_from_string(const char* src);

/**
 * cgs_string_free
 *
 * Deallocate a string.
 *
 * @param s	The string to be freed.
 */
void
cgs_string_free(struct cgs_string* s);

/**
 * cgs_string_read
 *
 * Get read-access to the string.
 *
 * @param s	The string to read.
 *
 * @return	A read-only pointer to the inner string.
 */
const char*
cgs_string_read(const struct cgs_string* s);

/**
 * cgs_string_length
 *
 * Get the length of the string.
 *
 * @param s	The string to get the length of.
 *
 * @return	The length of the string.
 */
size_t
cgs_string_length(const struct cgs_string* s);

/**
 * cgs_string_push
 *
 * Append a character to a string. Character is passed as an 'int' to match
 * return value of 'getchar' and 'fgetc'.
 *
 * @param s	The string to append to.
 * @param c	The character to append.
 *
 * @return	A read-only pointer to the inner string on success, NULL on
 * 		failure.
 */
const char*
cgs_string_push(struct cgs_string* s, int c);

/**
 * cgs_string_clear
 *
 * Clear the contents of the string and set the length to 0. Does not
 * deallocate the memory.
 *
 * **SECURITY WARNING**
 * - Does not erase the old contents of the string!
 * - Overwrites the first character to '\0' only.
 * - To completely overwrite the string call cgs_string_erase
 *
 * @param s	The string to clear.
 */
void
cgs_string_clear(struct cgs_string* s);

/**
 * cgs_string_erase
 *
 * Erases all of the data in the string and sets the length to 0. Does not
 * de-allocate memory.
 *
 * @param s	The string to erase.
 */
void
cgs_string_erase(struct cgs_string* s);
