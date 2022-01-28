#pragma once

/**
 * Variant
 *
 * A variable data type for use in tree data structures.
 */

enum cgs_type {
	CGS_TYPE_INT,
	CGS_TYPE_DOUBLE,
	CGS_TYPE_STRING,
	CGS_TYPE_DATA,
};

/**
 * union cgs_variant_data
 *
 * A union of the possible types for tree node data.
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
 * A union type that tracks it's contents.
 *
 * @member data		The union.
 * @member type		The data type of the union.
 */
struct cgs_variant {
	union cgs_data data;
	enum cgs_type type;
};

/**
 * cgs_variant_set_data
 *
 * Correctly set the provided data into the variant. Data is provided as a
 * pointer so literal values should not be passed.
 *
 * @param var	A pointer to a variant. This variant should already have its
 * 		type set by the caller.
 * @param p	A pointer to some data. The data will be interpreted based on
 * 		VAR's type member.
 */
void cgs_variant_set_data(struct cgs_variant* var, void* p);

/**
 * cgs_variant_get_data
 *
 * Get a read-only pointer to the variant data. Return value should be cast to
 * appropriate type by the caller.
 *
 * @param var	A read-only pointer to the variant.
 */
void* cgs_variant_get_data(struct cgs_variant* var);

