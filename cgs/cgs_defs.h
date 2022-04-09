/* cgs_defs.h
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Function-like Macros
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * CGS_MIN
 *
 * Evaluates the minimum of two numeric values. Checks each parameter
 * multiple times, beware of side-effects!
 *
 * @param a	Numeric value.
 * @param b	Numeric value.
 *
 * @result	The lesser of the two values.
 */
#define CGS_MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * CGS_MAX
 *
 * Evaluates the maximum of two numeric values. Checks each parameter
 * multiple times, beware of side-effects!
 *
 * @param a	Numeric value.
 * @param b	Numeric value.
 *
 * @result	The greater of the two values.
 */
#define CGS_MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * CGS_ARRAY_LENGTH
 *
 * Evalutates the length of a static array. **NO POINTERS**
 *
 * @param a	The array.
 *
 * @result	A positive value representing the length of the array.
 */
#define CGS_ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Typedefs
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * CgsStrIter
 *
 * This typedef simplifies a difficult to read type for pointers into const
 * arrays of const char*'s. This definition follows the principle of
 * const-by-default and does not indicate in its name that it is read-only.
 */
typedef const char* const* CgsStrIter;

/**
 * CgsStrIterMut
 *
 * This typedef is the mutable version of the const iterator. It's natural
 * type is actually not ugly to type or difficult to read so I recommend just
 * using 'char**' for this purpose.
 *
 * Defined here for symmetry of the interface.
 */
typedef char** CgsStrIterMut;

/**
 * CgsCmp3Way
 *
 * This comparison function matches the three-way, qsort compare signature.
 *
 * |Comparison|Return Value  |
 * |----------|--------------|
 * |  a < b   |Less than 0   |
 * |  a > b   |Greater than 0|
 * |  a == b  |0             |
 *
 */
typedef int (*CgsCmp3Way)(const void*, const void*);

