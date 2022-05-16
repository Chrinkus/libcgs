#include "cmocka_headers.h"

#include "cgs_sort.h"
#include "cgs_defs.h"

static void insertion_sort_test(void** state)
{
	(void)state;

	int rand_odd[] = { 5, 2, 4, 6, 1, 7, 3 };

	cgs_insertion_sort(rand_odd, CGS_ARRAY_LENGTH(rand_odd));

	assert_int_equal(rand_odd[0], 1);
	assert_int_equal(rand_odd[1], 2);
	assert_int_equal(rand_odd[2], 3);
	assert_int_equal(rand_odd[3], 4);
	assert_int_equal(rand_odd[4], 5);
	assert_int_equal(rand_odd[5], 6);
	assert_int_equal(rand_odd[6], 7);

	int desc[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	cgs_insertion_sort(desc, CGS_ARRAY_LENGTH(desc));

	assert_int_equal(desc[0], 1);
	assert_int_equal(desc[1], 2);
	assert_int_equal(desc[2], 3);
	assert_int_equal(desc[3], 4);
	assert_int_equal(desc[4], 5);
	assert_int_equal(desc[5], 6);
	assert_int_equal(desc[6], 7);
	assert_int_equal(desc[7], 8);
	assert_int_equal(desc[8], 9);
	assert_int_equal(desc[9], 10);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(insertion_sort_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

