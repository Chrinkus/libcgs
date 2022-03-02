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
 * @return	An allocated copy of S.
 */
char* cgs_strdup(const char* s);

/**
 * cgs_strappend
 *
 * Add a character to the end of a string. The caller is responsible for
 * ensuring there is enough room in 's'.
 *
 * @param s	The string to append to. Must have enough room allocated.
 * @param ch	The character to append.
 *
 * @return	Void.
 */
void cgs_strappend(char* s, char ch);

/**
 * cgs_strprepend
 *
 * Add a character to the front of a string. Caller is responsible for
 * ensuring there is enough room in 's'.
 *
 * @param s	The string to prepend to. Must have enough room allocated.
 * @param ch	The character to prepend.
 *
 * @return	Void.
 */
void cgs_strprepend(char* s, char ch);

/**
 * cgs_strshift
 *
 * Shift all characters towards the front of the string by 'n' characters.
 * Removes the first 'n' characters from the string.
 *
 * @param s	The string to shift.
 * @param n	Number of characters to shift. If this value is greater than
 * 		the length of the string then all characters are shifted out
 * 		and the string becomes empty. Passing -1 will also result in
 * 		an empty string.
 *
 * @return	Void.
 */
void cgs_strshift(char* s, size_t n);

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
