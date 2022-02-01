#pragma once

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
 * @return	An allocated copy of S.
 */
char* cgs_strdup(const char* s);

