/* cgs_sort.h
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

/* The following are implementations of common sorting algorithms. These have
 * been adapted from the examples in "Introduction to Algorithms, 3rd ed" by
 * Cormen et al.
 */

/**
 * cgs_insertion_sort
 *
 * Sort an array of integers using insertion sort. This algorithm works
 * similar to adding cards to a hand.
 *
 * Rather than having a hand and inserting cards individually, the entire
 * array is considered one element at a time starting from the second element.
 * Each element is then compared to all elements 'below' it in the array,
 * shifting them 'up' one index until a lesser element is found. The current
 * element is then inserted in the next location above the lesser element.
 *
 * @param arr	The array to be sorted.
 * @param len	Then length of the array.
 */
void
cgs_insertion_sort(int* arr, int len);

