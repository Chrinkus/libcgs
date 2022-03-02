#include "cgs_string_utils.h"
#include "cgs_test.h"

int strdup_test(void* data)
{
	const char* s1 = data;
	char* s2 = cgs_strdup(s1);

	assert(s2 != NULL);
	assert(s2 != s1);
	assert(strcmp(s1, s2) == 0);

	free(s2);

	const char* s3 = "";
	char* s4 = cgs_strdup(s3);

	assert(s3 != NULL);
	assert(s4 != s3);
	assert(strcmp(s3, s4) == 0);

	free(s4);

	return TEST_SUCCESS;
}

int strappend_test(void* data)
{
	(void)data;

	char s1[10] = "";

	cgs_strappend(s1, 'a');
	assert(strcmp(s1, "a") == 0);

	cgs_strappend(s1, 'b');
	assert(strcmp(s1, "ab") == 0);

	char* s2 = malloc(sizeof(char) * 10);
	*s2 = '\0';

	cgs_strappend(s2, '0');
	assert(strcmp(s2, "0") == 0);

	cgs_strappend(s2, '1');
	assert(strcmp(s2, "01") == 0);

	free(s2);

	return TEST_SUCCESS;
}

int strprepend_test(void* data)
{
	(void)data;

	char s1[10] = "mile";

	cgs_strprepend(s1, 's');
	assert(strcmp(s1, "smile") == 0);

	char* s2 = malloc(sizeof(char) * 10);
	*s2 = '\0';

	cgs_strprepend(s2, 't');
	cgs_strprepend(s2, 'a');
	cgs_strprepend(s2, 'o');
	cgs_strprepend(s2, 'g');
	assert(strcmp(s2, "goat") == 0);

	free(s2);

	return TEST_SUCCESS;
}

int strshift_test(void* data)
{
	(void)data;

	char s1[] = "Hello!";

	cgs_strshift(s1, 1);
	assert(strcmp(s1, "ello!") == 0);

	cgs_strshift(s1, 0);
	assert(strcmp(s1, "ello!") == 0);

	cgs_strshift(s1, 2);
	assert(strcmp(s1, "lo!") == 0);

	cgs_strshift(s1, 10);
	assert(strcmp(s1, "") == 0);

	char* s2 = malloc(sizeof(char) * 20);
	strcpy(s2, "less than 18");

	cgs_strshift(s2, 5);
	assert(strcmp(s2, "than 18") == 0);

	free(s2);

	char s3[] = "Such a long string! Who knows how many characters?!";
	cgs_strshift(s3, -1);
	assert(strcmp(s3, "") == 0);

	return TEST_SUCCESS;
}

int strtrim_test(void* data)
{
	(void)data;

	char s1[] = " \t trim it! \n\t \t";

	cgs_strtrim(s1);
	assert(strcmp(s1, "trim it!") == 0);

	char* s2 = malloc(sizeof(char) * 20);
	strcpy(s2, " typical\n");

	cgs_strtrim(s2);
	assert(strcmp(s2, "typical") == 0);

	free(s2);

	return TEST_SUCCESS;
}

int strtrimch_test(void* data)
{
	(void)data;

	char s1[] = "00Hello 0!000";

	cgs_strtrimch(s1, '0');
	assert(strcmp(s1, "Hello 0!") == 0);

	cgs_strtrimch(s1, 'H');
	assert(strcmp(s1, "ello 0!") == 0);
	cgs_strtrimch(s1, '!');
	assert(strcmp(s1, "ello 0") == 0);
	cgs_strtrimch(s1, 'l');
	assert(strcmp(s1, "ello 0") == 0);

	char* s2 = malloc(sizeof(char) * 20);
	strcpy(s2, "   christmas   ");

	cgs_strtrimch(s2, '-');
	assert(strcmp(s2, "   christmas   ") == 0);
	cgs_strtrimch(s2, ' ');
	assert(strcmp(s2, "christmas") == 0);

	free(s2);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "strdup", strdup_test, "Ninja doesn't like gnu_strdup" },
		{ "strappend", strappend_test, NULL },
		{ "strprepend", strprepend_test, NULL },
		{ "strshift", strshift_test, NULL },
		{ "strtrim", strtrim_test, NULL },
		{ "strtrimch", strtrimch_test, NULL },
	};

	return cgs_run_tests(tests);
}

