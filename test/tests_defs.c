#include "cmocka_headers.h"

#include "cgs_defs.h"

static void
defs_min_test(void** state)
{
	(void)state;

	assert_int_equal(CGS_MIN(5, 10), 5);
	assert_int_equal(CGS_MIN(5, 3), 3);
	assert_int_equal(CGS_MIN(-5, 3), -5);
	assert_int_equal(CGS_MIN(-5, -10), -10);

	assert_int_equal(CGS_MIN('a', 'b'), 'a');
	assert_int_equal(CGS_MIN('z', 'b'), 'b');
	assert_int_equal(CGS_MIN('0', '9'), '0');
	assert_int_equal(CGS_MIN('7', '2'), '2');
}

static void
defs_max_test(void** state)
{
	(void)state;

	assert_int_equal(CGS_MAX(5, 10), 10);
	assert_int_equal(CGS_MAX(5, 3), 5);
	assert_int_equal(CGS_MAX(-5, 3), 3);
	assert_int_equal(CGS_MAX(-5, -10), -5);

	assert_int_equal(CGS_MAX('a', 'b'), 'b');
	assert_int_equal(CGS_MAX('z', 'b'), 'z');
	assert_int_equal(CGS_MAX('0', '9'), '9');
	assert_int_equal(CGS_MAX('7', '2'), '7');
}

static void
defs_array_length_test(void** state)
{
	(void)state;

	int ai[] = { 1, 2, 3, 4, 5 };
	assert_int_equal(CGS_ARRAY_LENGTH(ai), 5);

	char ac[6] = { 'a', 'b', 'c' };
	assert_int_equal(CGS_ARRAY_LENGTH(ac), 6);

	double ad[0];
	assert_int_equal(CGS_ARRAY_LENGTH(ad), 0);
}

static void
defs_swap_test(void** state)
{
        (void)state;

        int ai[2] = { 67, 13, };
        CGS_SWAP(ai[0], ai[1], int);

        assert_int_equal(ai[0], 13);
        assert_int_equal(ai[1], 67);

        char* s1 = "Hello";
        char* s2 = "World";
        CGS_SWAP(s1, s2, char*);

        assert_string_equal(s1, "World");
        assert_string_equal(s2, "Hello");
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(defs_min_test),
		cmocka_unit_test(defs_max_test),
		cmocka_unit_test(defs_array_length_test),
		cmocka_unit_test(defs_swap_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
