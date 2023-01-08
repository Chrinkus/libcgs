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

/*
 * Type Constants
 */
enum cgs_type {
	CGS_VARIANT_TYPE_NULL = 0,
	CGS_VARIANT_TYPE_INT,
	CGS_VARIANT_TYPE_DOUBLE,
	CGS_VARIANT_TYPE_STRING,
	CGS_VARIANT_TYPE_DATA,
};

/**
 * union cgs_data
 *
 * A union of common data types.
 *
 * @member i	An integer type.
 * @member d	A double float type.
 * @member s	A char pointer to a string.
 * @member v	A pointer to arbitrary data.
 */
union cgs_data {
	int i;
	double d;
	char* s;
	void* v;
};

/**
 * struct cgs_variant
 *
 * A variable type for use in data structures.
 *
 * @member data		The union.
 * @member type		The data type of the union.
 */
struct cgs_variant {
	union cgs_data data;
	enum cgs_type type;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

/**
 * cgs_variant_set_NUMERIC
 *
 * Sets the type and data of the variant to the indicated NUMERIC type. The
 * value is stored directly in the variant.
 *
 * @param var	A pointer to the variant.
 * @param n	An integer value.
 */
void cgs_variant_set_int(struct cgs_variant* var, int n);
void cgs_variant_set_double(struct cgs_variant* var, double n);

/**
 * cgs_variant_set_string
 *
 * Sets the type and data of the variant to char*. The provided string is
 * copied into a separate allocation with strdup. The memory should be cleaned
 * up with cgs_variant_free_data or transferred out with cgs_variant_xfer.
 *
 * @param var	A pointer to the variant.
 * @param s	A string.
 */
void cgs_variant_set_string(struct cgs_variant* var, const char* s);

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
void cgs_variant_set_data(struct cgs_variant* var, void* v);

/**
 * cgs_variant_free_data
 *
 * Frees the data if string or void* data. If type is CGS_VARIANT_TYPE_DATA
 * and the data member requires non-trivial memory management, call
 * cgs_variant_xfer to surrender ownership of contained data and free
 * accordingly.
 *
 * DOES NOT FREE THE VARIANT. If the variant itself is allocated it must
 * be freed manually.
 *
 * @param var	A pointer to the variant.
 */
void cgs_variant_free_data(struct cgs_variant* var);

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
void* cgs_variant_xfer(struct cgs_variant* var);

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
const void* cgs_variant_get(const struct cgs_variant* var);

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
void* cgs_variant_get_mut(struct cgs_variant* var);

