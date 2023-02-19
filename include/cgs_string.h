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
#include <string.h>     /* strlen */

#include "cgs_vector.h"  /* vector for str_split */

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
 * Create and return a new cgs_string struct. All fields will be set to 0.
 *
 * @return      A new cgs_string struct.
 */
struct cgs_string
cgs_string_new(void);

/**
 * cgs_string_copy
 *
 * Copy a source string to a destination string.
 *
 * NOTE: Signature consists of `void*`'s to match CgsCopyFunc.
 *
 * @param s     A read-only pointer to the string to copy from.
 * @param d     A mutable pointer to the string to copy to.
 *
 * @return      A pointer to dst on successful allocation or NULL on failure.
 */
void*
cgs_string_copy(const void* s, void* d);

/**
 * cgs_string_move
 *
 * Move the members of one string to another and set the source string's data
 * to an empty string.
 * 
 * NOTE: Signature consists of `void*`'s to match CgsMoveFunc.
 *
 * @param src   The string to move from.
 * @param dst   The string to move to.
 */
void
cgs_string_move(void* s, void* d);

/**
 * cgs_string_from
 *
 * Create a new string by copying the contents of a standard C-string.
 *
 * @param src   A read-only pointer to the source C-string to copy from.
 * @param s     The destination string struct to allocate and copy to.
 *
 * @return      A pointer to s on successful allocation or NULL on failure.
 */
void*
cgs_string_from(const char* src, struct cgs_string* s);

/**
 * cgs_string_from_int
 *
 * Create a new string by converting an integer. Target string should be empty.
 *
 * @param n     The integer to convert.
 * @param s     The target string.
 *
 * @return      The target string on success, NULL on failure.
 */
void*
cgs_string_from_int(int n, struct cgs_string* s);

/**
 * cgs_string_free
 *
 * Deallocate a string.
 *
 * @param p     The string to be freed. Passed as `void*` to match free()
 *              signature.
 */
void
cgs_string_free(void* p);

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
 * cgs_string_data_mut
 *
 * Get mutable access to the string.
 *
 * @param s     The string to read.
 *
 * @return      A writable pointer to the data member of the string.
 */
inline char*
cgs_string_data_mut(struct cgs_string* s)
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
 * cgs_string_get_mut
 *
 * Get a mutable pointer to a given index in the string.
 *
 * @param s     The string struct.
 * @param i     The index of the pointer to get.
 *
 * @return      A writable pointer to the character at the given index.
 */
inline char*
cgs_string_get_mut(struct cgs_string* s, size_t i)
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
 * cgs_string_end_mut
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
cgs_string_end_mut(struct cgs_string* s)
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
 * cgs_string_begin_mut
 *
 * Get a writable pointer to the first character in the string. Identical to
 * `cgs_string_data_mutable`. Added for symmetry with `cgs_string_end_mutable`.
 *
 * @param s     The string.
 *
 * @return      A writable pointer to the first character in the string.
 */
inline char*
cgs_string_begin_mut(struct cgs_string* s)
{
        return s->data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String Standard Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * cgs_string_cmp
 *
 * A three-way comparison function for string structs. Strings passed as
 * void-pointers to mirror standard library signatures.
 *
 * @param a     A pointer to the target string.
 * @param b     A pointer to the compare string.
 *
 * @return      An integer indicating the result of the comparison with 0
 *              indicating equality.
 */
int
cgs_string_cmp(const void* a, const void* b);

/**
 * cgs_string_eq_str
 *
 * An equality test for comparing a cgs_string to a C-string. Parameters
 * passed as void*'s to match CgsPredicate signature.
 *
 * @param a     A pointer to a string.
 * @param b     A pointer to a C-string (const char*, const char[]).
 *
 * @return      A boolean integer indicating true(1) or false(0).
 */
int
cgs_string_eq_str(const void* a, const void* b);

/**
 * cgs_string_push
 *
 * Append a character to a string. Character is passed as an 'int' to match
 * return value of 'getchar' and 'fgetc'.
 *
 * @param s	The string to append to.
 * @param c	The character to append.
 *
 * @return	A pointer back to the provided string on success, NULL on
 * 		failure.
 */
void*
cgs_string_push(struct cgs_string* s, int c);

/**
 * cgs_string_cat
 *
 * Concatenate the contents of a string struct to the end of another string
 * struct.
 *
 * @param dst   The string struct to add to.
 * @param src   The string struct to add to the end of dst.
 *
 * @return      A pointer back to dst on success, NULL on failure.
 */
void*
cgs_string_cat(struct cgs_string* dst, const struct cgs_string* src);

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

/**
 * cgs_string_trunc
 *
 * Shortens a string to the given length by removing from the end. Does 
 * nothing if the string's length is already 'n' or less.
 *
 * @param s     The string to truncate.
 * @param n     The position at which the string will be truncated.
 */
void
cgs_string_trunc(struct cgs_string* s, size_t n);

/**
 * cgs_string_reverse
 *
 * Reverses the contents of a string in-place.
 *
 * @param s     The string to reverse.
 */
void
cgs_string_reverse(struct cgs_string* s);

/**
 * cgs_string_replace
 *
 * @param s     The string to replace within.
 * @param pos   The position in the string to start replacement.
 * @param count The number of characters to replace.
 * @param rep   The string to replace with.
 *
 * @return      The pointer 's' on success or NULL on failure.
 */
void*
cgs_string_replace(struct cgs_string* s, size_t pos, size_t count,
                const struct cgs_string* rep);

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
 * Strsub Creation
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

/**
 * cgs_strsub_from_str
 *
 * Create a cgs_strsub from a C-string.
 *
 * @param s     The C-string to map from.
 *
 * @return      A cgs_strsub of the whole C-string.
 */
inline struct cgs_strsub
cgs_strsub_from_str(const char* s)
{
        return (struct cgs_strsub){ .data = s, .length = strlen(s) };
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
 * cgs_strsub_eq_str
 *
 * An equality test for comparing a cgs_strsub to a C-string.
 *
 * @param a     A pointer to a strsub.
 * @param b     A pointer to a C-string (const char*, const char[]).
 *
 * @return      A boolean integer indicating true(1) or false(0).
 */
int
cgs_strsub_eq_str(const struct cgs_strsub* ss, const char* s);

/**
 * cgs_strsub_to_int
 *
 * Attempt to parse a sub-string as an integer.
 *
 * @param ss    The sub-string to parse.
 * @param out   A pointer to an integer to store the output in.
 *
 * @return      The 'out' pointer on successful parse, NULL on failure.
 */
void*
cgs_strsub_to_int(const struct cgs_strsub* ss, int* out);

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * String splitting functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * cgs_str_split
 *
 * Split the provided string into strsub's and store them in the provided
 * vector.
 *
 * @param s     The string to split.
 * @param delim The character to split the string on.
 * @param vec   The vector to store the strsub elements into.
 *
 * @return      A pointer back to the vector on success, NULL on failure.
 */
void*
cgs_str_split(const char* s, char delim, struct cgs_vector* vec);

/**
 * cgs_string_split
 *
 * Split the provided `struct cgs_string` into strsub's and store them in the
 * provided vector.
 *
 * @param s     The cgs_string to split.
 * @param delim The character to split the string on.
 * @param vec   The vector to store the strsub elements into.
 *
 * @return      A pointer back to the vector on success, NULL on failure.
 */
inline void*
cgs_string_split(const struct cgs_string* s, char delim, struct cgs_vector* v)
{
        return cgs_str_split(cgs_string_data(s), delim, v);
}

/**
 * cgs_strsub_split
 *
 * Split the provided sub-string further into strsub's and store them in the
 * provided vector.
 *
 * @param ss    The sub-string to split.
 * @param delim The character to split the string on.
 * @param vec   The vector to store the strsub elements into.
 *
 * @return      A pointer back to the vector on success, NULL on failure.
 */
void*
cgs_strsub_split(const struct cgs_strsub* ss, char delim,
                struct cgs_vector* vec);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * C-String Utility Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
void*
cgs_string_prepend_str(struct cgs_string* s, const char* add, size_t len);

/**
 * cgs_string_cat_str
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
void*
cgs_string_cat_str(struct cgs_string* s, const char* add, size_t len);

