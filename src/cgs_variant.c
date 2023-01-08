/* cgs_variant.c
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
	void* data = cgs_variant_get_mut(var);
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

void* cgs_variant_get_mut(struct cgs_variant* var)
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

