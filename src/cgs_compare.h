/* cgs_compare.h
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

/**
 * cgs_int_cmp
 *
 * Three-way comparison function for integers. Sorts from smallest to largest.
 *
 * @param a	A pointer to the target integer.
 * @param b	A pointer to the compare integer.
 *
 * @return	An integer indicating the result of the comparison.
 */
int cgs_int_cmp(const void* a, const void* b);

/**
 * cgs_int_cmp_rev
 *
 * Three-wat comparison function for integers. Sorts from largest to smallest.
 *
 * @param a	A pointer to the target integer.
 * @param b	A pointer to the compare integer.
 *
 * @return	An integer indicating the result of the comparison.
 */
int cgs_int_cmp_rev(const void* a, const void* b);

/**
 * cgs_str_cmp
 *
 * Three-way comparison function for strings (char*, char[]).
 *
 * @param a	A pointer to the target string.
 * @param b	A pointer to the compare string.
 *
 * @return	An integer indicating the result of the comparison.
 */
int cgs_str_cmp(const void* a, const void* b);

/**
 * cgs_char_cmp
 *
 * Three-way comparison function for chars.
 *
 * @param a	A pointer to the target char.
 * @param b	A pointer to the compare char.
 *
 * @return	An integer indicating the result of the comparison.
 */
int cgs_char_cmp(const void* a, const void* b);

/**
 * cgs_int_pred
 *
 * True or false comparison function for integers.
 *
 * @param a     A pointer to the target integer.
 * @param b     A pointer to the compare integer.
 *
 * @return      The result of an equality comparison.
 */
int cgs_int_pred(const void* a, const void* b);

