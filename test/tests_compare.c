#include "cmocka_headers.h"

#include "cgs_compare.h"

#include <limits.h>

static void compare_int_test(void** state)
{
	(void)state;

	int a = 2;
	int b = 2;
	int c = -27;
	int d = 16;
	int e = 200;
	int f = 32;

	assert_true(cgs_int_cmp(&a, &b) == 0);
	assert_true(cgs_int_cmp(&c, &d) < 0);
	assert_true(cgs_int_cmp(&e, &f) > 0);

	int g = -2000000000;
	int h = 2000000000;

	assert_true(cgs_int_cmp(&g, &h) < 0);
	assert_true(cgs_int_cmp(&h, &g) > 0);

	int i = INT_MIN;
	int j = INT_MAX;

	assert_true(cgs_int_cmp(&i, &j) < 0);
	assert_true(cgs_int_cmp(&j, &i) > 0);
}

static void
compare_int_rev_test(void** state)
{
        (void)state;

	int a = 2;
	int b = 2;
	int c = -27;
	int d = 16;
	int e = 200;
	int f = 32;

	assert_true(cgs_int_cmp_rev(&a, &b) == 0);
	assert_true(cgs_int_cmp_rev(&c, &d) > 0);
	assert_true(cgs_int_cmp_rev(&e, &f) < 0);

	int g = -2000000000;
	int h = 2000000000;

	assert_true(cgs_int_cmp_rev(&g, &h) > 0);
	assert_true(cgs_int_cmp_rev(&h, &g) < 0);

	int i = INT_MIN;
	int j = INT_MAX;

	assert_true(cgs_int_cmp_rev(&i, &j) > 0);
	assert_true(cgs_int_cmp_rev(&j, &i) < 0);
}

static void compare_char_test(void** state)
{
	(void)state;

	char a = 'j';
	char b = 'j';
	char c = '0';
	char d = '9';
	char e = 'T';
	char f = 'C';

	assert_true(cgs_char_cmp(&a, &b) == 0);
	assert_true(cgs_char_cmp(&c, &d) < 0);
	assert_true(cgs_char_cmp(&e, &f) > 0);
}

static void compare_str_test(void** state)
{
	(void)state;

	const char* a = "Howdy";
	const char* b = "Howdy";
	const char* c = "less";
	const char* d = "more";
	const char* e = "longer";
	const char* f = "long";

	assert_true(cgs_str_cmp(&a, &b) == 0);
	assert_true(cgs_str_cmp(&c, &d) < 0);
	assert_true(cgs_str_cmp(&e, &f) > 0);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(compare_int_test),
		cmocka_unit_test(compare_int_rev_test),
		cmocka_unit_test(compare_char_test),
		cmocka_unit_test(compare_str_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

