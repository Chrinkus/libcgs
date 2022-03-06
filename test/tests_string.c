#include "cgs_string.h"
#include "cgs_test.h"

#include <string.h>

int string_new_test(void* data)
{
	(void)data;

	struct cgs_string* s = cgs_string_new();

	assert(s != NULL);
	assert(cgs_string_length(s) == 0);
	assert(strcmp(cgs_string_read(s), "") == 0);

	cgs_string_free(s);

	return TEST_SUCCESS;
}

int string_new_from_string_test(void* data)
{
	const char* test = (const char*)data;

	struct cgs_string* s = cgs_string_new_from_string(test);

	assert(s != NULL);
	assert(cgs_string_length(s) == strlen(test));
	assert(strcmp(cgs_string_read(s), test) == 0);
	assert(cgs_string_read(s) != test);

	cgs_string_push(s, '!');
	assert(strcmp(cgs_string_read(s), test) != 0);

	cgs_string_free(s);

	return TEST_SUCCESS;
}

int string_push_test(void* data)
{
	(void)data;

	struct cgs_string* s = cgs_string_new();

	cgs_string_push(s, 'p');
	cgs_string_push(s, 'u');
	cgs_string_push(s, 's');
	cgs_string_push(s, 'h');

	assert(cgs_string_length(s) == 4);
	assert(strcmp(cgs_string_read(s), "push") == 0);

	cgs_string_free(s);

	return TEST_SUCCESS;
}

int string_clear_test(void* data)
{
	(void)data;

	struct cgs_string* s = cgs_string_new_from_string("Howdy");

	cgs_string_clear(s);

	assert(cgs_string_length(s) == 0);
	assert(cgs_string_read(s) != NULL);

	// 'clear' just changes first char to '\0'
	assert(cgs_string_read(s)[1] == 'o');
	assert(cgs_string_read(s)[2] == 'w');
	assert(cgs_string_read(s)[3] == 'd');
	assert(cgs_string_read(s)[4] == 'y');
	assert(cgs_string_read(s)[5] == '\0');

	cgs_string_free(s);

	return TEST_SUCCESS;
}

int string_erase_test(void* data)
{
	(void)data;

	struct cgs_string* s = cgs_string_new_from_string("Howdy");

	cgs_string_erase(s);

	assert(cgs_string_length(s) == 0);
	assert(cgs_string_read(s) != NULL);

	// 'erase' overwrites all characters with '\0'
	assert(cgs_string_read(s)[1] == '\0');
	assert(cgs_string_read(s)[2] == '\0');
	assert(cgs_string_read(s)[3] == '\0');
	assert(cgs_string_read(s)[4] == '\0');
	assert(cgs_string_read(s)[5] == '\0');

	cgs_string_free(s);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "string_new", string_new_test, NULL },
		{ "string_push", string_push_test, NULL },
		{ "string_clear", string_clear_test, NULL },
		{ "string_erase", string_erase_test, NULL },
		{ "string_new_from_string", string_new_from_string_test,
			"Super test string" },
	};

	return cgs_run_tests(tests);
}
