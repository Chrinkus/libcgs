/* cgs_string_utils.h
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

/**
 * String Utils
 *
 * Some string utilities that are either not part of the C standard library or
 * not available on all implementations.
 */

/**
 * cgs_strdup
 *
 * Allocate a copy of the provided string and return it.
 *
 * @param s	The string to copy.
 *
 * @return	An allocated copy of s on success, NULL on failure.
 */
char* cgs_strdup(const char* s);

/**
 * cgs_strmove
 *
 * Moves the start of the string 'n' characters along the string. The caller
 * is responsible for ensuring there is enough room in 's'.
 *
 * Note: Differs from 'cgs_strshiftr' in that the end of the string is not
 * truncated.
 *
 * @param s     The string to move.
 * @param n     The number of character places to move the start to.
 */
void cgs_strmove(char* s, size_t n);

/**
 * cgs_strprepend
 *
 * Add a string to the start of another. The caller is responsible for
 * ensuring there is enough room in the destination string.
 *
 * @param dst   The destination string to prepend to.
 * @param src   The source string to  prepend.
 * @param len   The number of chars from the start of the 'src' string to
 *              prepend. May be shorter than strlen(src).
 */
void cgs_strprepend(char* dst, const char* src, size_t n);

/**
 * cgs_strappendch
 *
 * Add a character to the end of a string. The caller is responsible for
 * ensuring there is enough room in 's'.
 *
 * @param s	The string to append to. Must have enough room allocated.
 * @param ch	The character to append.
 */
void cgs_strappendch(char* s, char ch);

/**
 * cgs_strprependch
 *
 * Add a character to the front of a string. Caller is responsible for
 * ensuring there is enough room in 's'.
 *
 * @param s	The string to prepend to. Must have enough room allocated.
 * @param ch	The character to prepend.
 */
void cgs_strprependch(char* s, char ch);

/**
 * cgs_strshiftl
 *
 * Shift all characters 'left' towards the front of the string by 'n'
 * characters. Removes the first 'n' characters from the string. The character
 * passed as 'c' will be used to fill the space vacated at the end of the
 * string.
 *
 * @param s	The string to shift.
 * @param n	Number of characters to shift. If this value is greater than
 * 		the length of the string then all characters are shifted out
 * 		and the string becomes empty.
 * @param c     The character to shift 'in' to the space vacated.
 */
void cgs_strshiftl(char* s, size_t n, int c);

/**
 * cgs_strshiftr
 *
 * Shift all characters 'right' towards the end of the string by 'n'
 * characters. Removes the last 'n' characters from the string. The character
 * passed as 'c' will be used to fill the space vacated at the beginning of
 * the string.
 *
 * @param s     The  string to shift.
 * @param n     Number of characters to shift. If this value is greater than
 *              the length of the string then all characters are shifted out
 *              and the string becomes empty.
 * @param c     The character to shift 'in' to the space vacated.
 */
void cgs_strshiftr(char* s, size_t n, int c);

/**
 * cgs_strtrim
 *
 * Remove all whitespace from the beginning and end of a string.
 *
 * @param s	The string to trim.
 *
 * @return	Void.
 */
void cgs_strtrim(char* s);

/**
 * cgs_strtrimch
 *
 * Remove all instances of 'ch' from the beginning and end of a string.
 *
 * @param s	The string to trim.
 * @param ch	The character to remove.
 *
 * @return	Void.
 */
void cgs_strtrimch(char* s, char ch);
