#include "cgs_string_utils.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* cgs_strdup(const char* src)
{
	int len = strlen(src) + 1;
	char* dst = malloc(len * sizeof(src[0]));

	if (dst)
		strcpy(dst, src);
	return dst;
}

void cgs_strappend(char* s, char ch)
{
	while (*s)
		++s;
	*s++ = ch;
	*s = '\0';
}

void cgs_strprepend(char* s, char ch)
{
	for (char tmp = *s; ch; *s++ = ch, ch = tmp)
		if (tmp)
			tmp = *s;
	*s = '\0';
}

void cgs_strshift(char* s, size_t n)
{
	if (strlen(s) < n) {
		*s = '\0';
	} else {
		for (char* p = s+n; *s; ++s, ++p)
			*s = *p;
	}
}

void cgs_strtrim(char* s)
{
	char* p = s;
	while (isspace(*p))
		++p;

	while (*p)
		*s++ = *p++;

	do {
		*s = '\0';
	} while (isspace(*--s));
}

void cgs_strtrimch(char* s, char ch)
{
	char* p = s;
	while (*p == ch)
		++p;

	while (*p)
		*s++ = *p++;

	do {
		*s = '\0';
	} while (*--s == ch);
}

