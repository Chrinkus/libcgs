#pragma once

/**
 * Comparison Functions
 *
 * These comparison functions act as 3-way, qsort-compatible comparisons.
 *
 * |Comparison|Return Value  |
 * |----------|--------------|
 * |  a < b   |Less than 0   |
 * |  a > b   |Greater than 0|
 * |  a == b  |0             |
 *
 */

/**
 * cgs_int_cmp
 *
 * Three-way comparison function for integers.
 *
 * @param a	A pointer to the target integer.
 * @param b	A pointer to the compare integer.
 *
 * @return	An integer indicating the result of the comparison.
 */
int cgs_int_cmp(const void* a, const void* b);

/**
 * cgs_int_cmp
 *
 * Three-way comparison function for strings (char*, char[]).
 *
 * @param a	A pointer to the target string.
 * @param b	A pointer to the compare string.
 *
 * @return	An integer indicating the result of the comparison.
 */
int cgs_str_cmp(const void* a, const void* b);

