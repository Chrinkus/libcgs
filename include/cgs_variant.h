/* cgs_variant.h
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

#include "cgs_defs.h"

/*
 * Type Constants
 */
enum cgs_type {
	CGS_VARIANT_TYPE_NULL = 0,
	CGS_VARIANT_TYPE_INT,
	CGS_VARIANT_TYPE_LONG,
	CGS_VARIANT_TYPE_UINT,
	CGS_VARIANT_TYPE_ULONG,
	CGS_VARIANT_TYPE_FLOAT,
	CGS_VARIANT_TYPE_DOUBLE,
	CGS_VARIANT_TYPE_C_STR,
	CGS_VARIANT_TYPE_DATA,
};

/**
 * union cgs_data
 *
 * A union of common data types.
 *
 * @member i	An integer type.
 * @member l	A long integer type.
 * @member u	An unsigned integer type.
 * @member ul   An unsigned long integer type.
 * @member f	A float type.
 * @member d	A double float type.
 * @member s	A char pointer to a c-string.
 * @member v	A pointer to arbitrary data.
 */
union cgs_data {
	int i;
        long l;
        unsigned u;
        unsigned long ul;
        float f;
	double d;
	char* s;
	void* v;
};

/**
 * struct cgs_variant
 *
 * A variable type for use in data structures.
 *
 * @member type		The data type of the union.
 * @member data		The union.
 */
struct cgs_variant {
	enum cgs_type type;
	union cgs_data data;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

/**
 * cgs_variant_set_NUMERIC
 *
 * Sets the type and data of the variant to the indicated NUMERIC type. The
 * value is stored directly in the variant.
 *
 * @param var	A pointer to the variant.
 * @param n	A numeric value.
 */
void
cgs_variant_set_int(struct cgs_variant* var, int n);
void
cgs_variant_set_long(struct cgs_variant* var, long n);
void
cgs_variant_set_uint(struct cgs_variant* var, unsigned n);
void
cgs_variant_set_ulong(struct cgs_variant* var, unsigned long n);
void
cgs_variant_set_float(struct cgs_variant* var, float n);
void
cgs_variant_set_double(struct cgs_variant* var, double n);

/**
 * cgs_variant_set_cstr
 *
 * Sets the type and data of the variant to char*. The provided string is
 * copied into a separate allocation with strdup. The memory should be cleaned
 * up with cgs_variant_free or transferred out with cgs_variant_xfer.
 *
 * @param var	A pointer to the variant.
 * @param s	A c-string.
 */
void
cgs_variant_set_cstr(struct cgs_variant* var, const char* s);

/**
 * cgs_variant_set_data
 *
 * Sets the type and data of the variant to a generic void*. The provided
 * data pointer is assumed to be allocated on the heap and is assigned to the
 * variant as-is. The variant assumes ownership of the memory.
 *
 * @param var	A pointer to the variant.
 * @param v	A pointer to some dynamically allocated data.
 */
void
cgs_variant_set_data(struct cgs_variant* var, void* v);

/**
 * cgs_variant_free
 *
 * Frees the data if string or void* data, does nothing when called on other
 * variant types. Calls standard library `free` on the contained pointer. If
 * the data value is non-trivially freed, an optional freeing function may be
 * passed that will be called on the object first.
 *
 * DOES NOT FREE THE VARIANT. If the variant itself is allocated it must
 * be freed manually.
 *
 * @param var	A pointer to the variant.
 * @param ff    An optional function to free the data value with before
 *              freeing the data object or NULL.
 */
void
cgs_variant_free(struct cgs_variant* var, CgsFreeFunc ff);

/**
 * cgs_variant_xfer
 *
 * Transfers ownership of the data out of the variant. Sets variant type to
 * CGS_VARIANT_TYPE_NULL and data to NULL. Only use for allocated variant types
 * (STRING and DATA).
 *
 * @param var	A pointer to the variant.
 *
 * @return	A pointer to the variant data. Data will no longer be 
 * 		accessible through the variant.
 */
void*
cgs_variant_xfer(struct cgs_variant* var);

/**
 * cgs_variant_get
 *
 * Get a read-only pointer to the variant data. 
 *
 * @param var	A pointer to the variant.
 *
 * @return	A read-only pointer to the variant data aligned according to
 * 		variant type.
 */
const void*
cgs_variant_get(const struct cgs_variant* var);

/**
 * cgs_variant_get_mut
 *
 * Get a mutable pointer to the variant data.
 *
 * @param var	A pointer to the variant.
 *
 * @return	A mutable pointer to the variant data aligned according to
 * 		variant type.
 */
void*
cgs_variant_get_mut(struct cgs_variant* var);
