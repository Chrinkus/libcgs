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
 * String Type
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_string
 *
 * A dynamic string.
 *
 * @member length       The number of characters in the string.
 * @member capacity     The number of characters that the string has room for
 *                      plus 1 for the terminating '\0'.
 * @member data         A pointer to the allocation.
 */
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
 * cgs_string_new_from_str
 *
 * Create a new string by copying the contents of a standard string.
 *
 * @param s     The destination string struct to allocate and copy to.
 * @param src   A read-only pointer to the source string to copy from.
 *
 * @return      A valid pointer on successful allocation or NULL on failure.
 */
void*
cgs_string_new_from_str(struct cgs_string* s, const char* src);

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

/**
 * cgs_string_get
 *
 * Get a read-only pointer to a given index in the string.
 *
 * @param s     The string struct.
 * @param i     The index of the pointer to get.
 *
 * @return      A read-only pointer to the character at the given index.
 */
inline const char*
cgs_string_get(const struct cgs_string* s, size_t i)
{
        return &s->data[i];
}

/**
 * cgs_string_get_mutable
 *
 * Get a writable pointer to a given index in the string.
 *
 * @param s     The string struct.
 * @param i     The index of the pointer to get.
 *
 * @return      A writable pointer to the character at the given index.
 */
inline char*
cgs_string_get_mutable(struct cgs_string* s, size_t i)
{
        return &s->data[i];
}

/**
 * cgs_string_char
 *
 * Get the character at a given index in the string.
 *
 * @param s     The string struct.
 * @param i     The index of the character to get.
 *
 * @return      The character at the given index.
 */
inline char
cgs_string_char(const struct cgs_string* s, size_t i)
{
        return s->data[i];
}

/**
 * cgs_string_end
 *
 * Get a read-only pointer to the character past the last character in the
 * string. Should be a null byte '\0'.
 *
 * @param s     The string.
 *
 * @return      A read-only pointer to the character past the last character
 *              in the string.
 */
inline const char*
cgs_string_end(const struct cgs_string* s)
{
        return &s->data[s->length];
}

/**
 * cgs_string_end_mutable
 *
 * Get a writable pointer to the character past the last character in the
 * string. Should be a null byte '\0'.
 *
 * @param s     The string.
 *
 * @return      A writable pointer to the character past the last character
 *              in the string.
 */
inline char*
cgs_string_end_mutable(struct cgs_string* s)
{
        return &s->data[s->length];
}

/**
 * cgs_string_begin
 *
 * Get a read-only pointer to the first character in the string. Identical to
 * `cgs_string_data`. Added for symmetry with `cgs_string_end`.
 *
 * @param s     The string.
 *
 * @return      A read-only pointer to the first character in the string.
 */
inline const char*
cgs_string_begin(const struct cgs_string* s)
{
        return s->data;
}

/**
 * cgs_string_begin_mutable
 *
 * Get a writable pointer to the first character in the string. Identical to
 * `cgs_string_data_mutable`. Added for symmetry with `cgs_string_end_mutable`.
 *
 * @param s     The string.
 *
 * @return      A writable pointer to the first character in the string.
 */
inline char*
cgs_string_begin_mutable(struct cgs_string* s)
{
        return s->data;
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Strsub Type
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * struct cgs_strsub
 *
 * A read-only sub-string view into another string.
 *
 * WARNING: Does not own the target string, any changes to the target string
 * may invalidate the data pointer.
 *
 * @member data         A pointer to the start of the sub-string.
 * @member length       The number of characters in the sub-string.
 */
struct cgs_strsub {
        const char* data;
        size_t length;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Strsub Management
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_strsub_new
 *
 * Creates a new sub-string using the pointer and length given.
 *
 * @param s     A pointer into a string that will be the start of the
 *              sub-string.
 * @param len   The length of the sub-string.
 *
 * @return      A sub-string object.
 */
inline struct cgs_strsub
cgs_strsub_new(const char* s, size_t len)
{
        return (struct cgs_strsub){ .data = s, .length = len };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Strsub Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_strsub_cmp
 *
 * A three-way comparison function for cgs_strsub's.
 *
 * @param a     A pointer to the target strsub.
 * @param b     A pointer to the compare strsub.
 *
 * @return      An integer indicating equality(0), lesser(<0) or greater(>0).
 */
int
cgs_strsub_cmp(const void* a, const void* b);

/**
 * cgs_strsub_to_str
 *
 * Allocate a duplicate of the provided sub-string.
 *
 * @param ss    The sub-string to duplicate.
 *
 * @return      An allocated copy of the sub-string on success, NULL on
 *              failure. Caller is responsible for freeing.
 */
char*
cgs_strsub_to_str(const struct cgs_strsub* ss);

/**
 * cgs_strsub_to_string
 *
 * Duplicate a sub-string in the provided cgs_string.
 *
 * @param ss    The sub-string to duplicate.
 * @param dst   The cgs_string struct to duplicate into.
 *
 * @return      A pointer back to 'dst' on success, NULL on failure.
 */
void*
cgs_strsub_to_string(const struct cgs_strsub* ss, struct cgs_string* dst);
