#include "cgs_variant.h"
#include "cgs_string_utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void cgs_variant_set_int(struct cgs_variant* var, int n)
{
	var->type = CGS_VARIANT_TYPE_INT;
	var->data.i = n;
}

void cgs_variant_set_double(struct cgs_variant* var, double x)
{
	var->type = CGS_VARIANT_TYPE_DOUBLE;
	var->data.d = x;
}

void cgs_variant_set_string(struct cgs_variant* var, const char* s)
{
	var->type = CGS_VARIANT_TYPE_STRING;
	var->data.s = cgs_strdup(s);
}

void cgs_variant_set_data(struct cgs_variant* var, void* data)
{
	var->type = CGS_VARIANT_TYPE_DATA;
	var->data.v = data;
}

void cgs_variant_free_data(struct cgs_variant* var)
{
	switch (var->type) {
	case CGS_VARIANT_TYPE_NULL:	/* fallthrough */
	case CGS_VARIANT_TYPE_INT:	/* fallthrough */
	case CGS_VARIANT_TYPE_DOUBLE:	/* do  nothing */	break;

	case CGS_VARIANT_TYPE_STRING:	free(var->data.s);	break;
	case CGS_VARIANT_TYPE_DATA:	free(var->data.v);	break;
	}
	var->type = CGS_VARIANT_TYPE_NULL;
}

void* cgs_variant_xfer(struct cgs_variant* var)
{
	void* data = cgs_variant_get_mutable(var);
	var->type = CGS_VARIANT_TYPE_NULL;
	var->data.v = NULL;
	return data;
}

const void* cgs_variant_get(const struct cgs_variant* var)
{
	switch (var->type) {
	case CGS_VARIANT_TYPE_INT:	return (void*)&var->data.i;
	case CGS_VARIANT_TYPE_DOUBLE:	return (void*)&var->data.d;
	case CGS_VARIANT_TYPE_STRING:	return (void*)var->data.s;
	case CGS_VARIANT_TYPE_DATA:	return var->data.v;

	case CGS_VARIANT_TYPE_NULL:	/* fallthrough */
	default:			return NULL;
	}
}

void* cgs_variant_get_mutable(struct cgs_variant* var)
{
	/*
	 * This is dirty. I tried reversing these and const-casting the
	 * result from _mutable but the compiler complained. Apparently
	 * this is acceptable..
	 */
	return (void*)cgs_variant_get(var);
}

