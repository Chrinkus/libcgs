#include "cgs_compare.h"
#include "cgs_test.h"

int compare_int_test(void* data)
{
	(void)data;

	int a = 2;
	int b = 2;
	int c = -27;
	int d = 16;
	int e = 200;
	int f = 32;

	assert(cgs_int_cmp(&a, &b) == 0);
	assert(cgs_int_cmp(&c, &d) < 0);
	assert(cgs_int_cmp(&e, &f) > 0);

	return TEST_SUCCESS;
}

int compare_char_test(void* data)
{
	(void)data;

	char a = 'j';
	char b = 'j';
	char c = '0';
	char d = '9';
	char e = 'T';
	char f = 'C';

	assert(cgs_char_cmp(&a, &b) == 0);
	assert(cgs_char_cmp(&c, &d) < 0);
	assert(cgs_char_cmp(&e, &f) > 0);

	return TEST_SUCCESS;
}

int compare_str_test(void* data)
{
	(void)data;

	const char* a = "Howdy";
	const char* b = "Howdy";
	const char* c = "less";
	const char* d = "more";
	const char* e = "longer";
	const char* f = "long";

	assert(cgs_str_cmp(&a, &b) == 0);
	assert(cgs_str_cmp(&c, &d) < 0);
	assert(cgs_str_cmp(&e, &f) > 0);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "compare_int", compare_int_test, NULL },
		{ "compare_char", compare_char_test, NULL },
		{ "compare_str", compare_str_test, NULL },
	};

	return cgs_run_tests(tests);
}

