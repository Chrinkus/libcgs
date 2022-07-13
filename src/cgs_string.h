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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct cgs_string {
        size_t length;
        size_t capacity;
        char* data;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_string_new
 *
 * Initialize and allocate a new cgs_string struct. Length will be 0, capacity
 * will be set to a reasonable default and the first character will be '\0'.
 *
 * @param s     The string struct to initialize.
 *
 * @return      A valid pointer on successful allocation or NULL on failure.
 */
void*
cgs_string_new(struct cgs_string* s);

/**
 * cgs_string_new_from_string
 *
 * Create a new string by copying the contents of a standard string.
 *
 * @param s     The destination string struct to allocate and copy to.
 * @param src   A read-only pointer to the source string to copy from.
 *
 * @return      A valid pointer on successful allocation or NULL on failure.
 */
void*
cgs_string_new_from_string(struct cgs_string* s, const char* src);

/**
 * cgs_string_free
 *
 * Deallocate a string.
 *
 * @param s     The string to be freed.
 */
void
cgs_string_free(struct cgs_string* s);

/**
 * cgs_string_shrink
 *
 * Shrinks the allocation of a string to the smallest necessary size.
 *
 * @param s     The string struct to shrink.
 *
 * @return      A pointer to the string if shrink was successful, NULL on
 *              failure.
 */
void*
cgs_string_shrink(struct cgs_string* s);

/**
 * cgs_string_xfer
 *
 * Release ownership of the inner string buffer.
 *
 * @param s     The string struct to release ownership from.
 *
 * @return      A pointer to the transferred memory.
 */
char*
cgs_string_xfer(struct cgs_string* s);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_string_data
 *
 * Get read-only access to the string.
 *
 * @param s     The string to read.
 *
 * @return      A read-only pointer to the data member of the string.
 */
inline const char*
cgs_string_data(const struct cgs_string* s)
{
        return s->data;
}

/**
 * cgs_string_data
 *
 * Get writable access to the string.
 *
 * @param s     The string to read.
 *
 * @return      A writable pointer to the data member of the string.
 */
inline char*
cgs_string_data_mutable(struct cgs_string* s)
{
        return s->data;
}

/**
 * cgs_string_length
 *
 * Get the length of the string.
 *
 * @param s     The string to get the length of.
 *
 * @return      The length of the string.
 */
inline size_t
cgs_string_length(const struct cgs_string* s)
{
        return s->length;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

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
 * cgs_string_prepend_str
 *
 * Insert a standard C-string to the beginning of a string struct. May result
 * in a reallocation.
 *
 * @param s     The string to prepend to.
 * @param add   The standard string to add to the front of s.
 * @param len   The length of the add string.
 *
 * @return	A pointer back to s on success, NULL on failure.
 */
struct cgs_string*
cgs_string_prepend_str(struct cgs_string* s, const char* add, size_t len);

/**
 * cgs_string_append_str
 *
 * Insert a standard C-string to the end of a string struct. May result in a
 * reallocation.
 *
 * @param s     The string to append to.
 * @param add   The standard string to add to the end of s.
 * @param len   The length of the add string.
 *
 * @return	A pointer back to s on success, NULL on failure.
 */
struct cgs_string*
cgs_string_append_str(struct cgs_string* s, const char* add, size_t len);

/**
 * cgs_string_prepend
 *
 * Insert the contents of a string struct to the beginning of another string
 * struct. May result in an reallocation.
 *
 * @param s     The string struct to add to.
 * @param add   The string struct to add to the front of s.
 *
 * @return      A pointer back to s on success, NULL on failure.
 */
struct cgs_string*
cgs_string_prepend(struct cgs_string* s, const struct cgs_string* add);

/**
 * cgs_string_prepend
 *
 * Insert the contents of a string struct to the end of another string struct.
 * May result in an reallocation.
 *
 * @param s     The string struct to add to.
 * @param add   The string struct to add to the end of s.
 *
 * @return      A pointer back to s on success, NULL on failure.
 */
struct cgs_string*
cgs_string_append(struct cgs_string* s, const struct cgs_string* add);

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

/**
 * cgs_string_sort
 *
 * Sorts the characters of the string in-place.
 *
 * @param s	The string to sort.
 */
void
cgs_string_sort(struct cgs_string* s);

