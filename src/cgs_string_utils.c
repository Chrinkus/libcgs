#include "cgs_string_utils.h"

#include <stdlib.h>
#include <string.h>

char* cgs_strdup(const char* src)
{
	int len = strlen(src) + 1;
	char* dst = malloc(len * sizeof(src[0]));

	if (dst)
		strcpy(dst, src);
	return dst;
}

