#include "config.h"

#include "cgs_variant.h"

#include <stddef.h>

void cgs_variant_set_data(struct cgs_variant* var, void* p)
{
	switch (var->type) {
	case CGS_TYPE_INT:
		var->data.i = *(int*)p;
		break;
	case CGS_TYPE_DOUBLE:
		var->data.d = *(double*)p;
		break;
	case CGS_TYPE_STRING:
		var->data.s = (char*)p;
		break;
	case CGS_TYPE_DATA:
		var->data.v = p;
		break;
	}
}

void* cgs_variant_get_data(struct cgs_variant* var)
{
	switch (var->type) {
	case CGS_TYPE_INT:	return (void*)&var->data.i;
	case CGS_TYPE_DOUBLE:	return (void*)&var->data.d;
	case CGS_TYPE_STRING:	return (void*)var->data.s;
	case CGS_TYPE_DATA:	return var->data.v;
	}
}
