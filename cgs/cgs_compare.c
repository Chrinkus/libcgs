#include "cgs_compare.h"

#include <string.h>

int cgs_int_cmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int cgs_str_cmp(const void* a, const void* b)
{
	const char* s1 = *(const char**)a;
	const char* s2 = *(const char**)b;

	return strcmp(s1, s2);
}

