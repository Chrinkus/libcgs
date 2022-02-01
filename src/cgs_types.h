#pragma once

/*
 * WIP - As I need more types across source files I'll figure out a way to
 * organize this.
 */

/**
 * cgs_cmp_3way
 *
 * A three-way comparison function. Shares the same signature as qsort compare.
 */
typedef int (*cgs_cmp_3way)(const void*, const void*);

