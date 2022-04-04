#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "cgs_array.h"

#include "cgs_compare.h"
#include "cgs_string_utils.h"

int test_int_iota[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

enum { RANDOM_SIZE = 10 };

static int setup_random(void** state)
{
	int* ai = malloc(sizeof(int) * RANDOM_SIZE);
	if (!ai)
		return -1;
	ai[0] = 37;
	ai[1] = 2;
	ai[2] = 10;
	ai[3] = -13;
	ai[4] = -22;
	ai[5] = 5;
	ai[6] = 18;
	ai[7] = -1;
	ai[8] = 0;
	ai[9] = 30;

	*state = ai;

	return 0;
}

static int teardown_random(void** state)
{
	free(*state);
	return 0;
}

static void array_new_test(void** state)
{
	(void)state;

	struct cgs_array* ai = cgs_array_new(int);

	assert_non_null(ai);
	assert_int_equal(cgs_array_length(ai), 0);

	cgs_array_free(ai);

	struct cgs_array* ad = cgs_array_new(double);
	assert_non_null(ad);
	assert_int_equal(cgs_array_length(ad), 0);

	cgs_array_free(ad);

	struct cgs_array* as = cgs_array_new(char*);
	assert_non_null(as);
	assert_int_equal(cgs_array_length(as), 0);

	cgs_array_free(as);
}

static void array_push_test(void** state)
{
	const int* ints = *(const int**)state;

	struct cgs_array* ai = cgs_array_new(int);
	for (int i = 0; i < RANDOM_SIZE; ++i)
		cgs_array_push(ai, &ints[i]);

	assert_int_equal(cgs_array_length(ai), RANDOM_SIZE);

	cgs_array_free(ai);
}

static void array_sort_test(void** state)
{
	const int* ints = *(const int**)state;

	struct cgs_array* ai = cgs_array_new_from_array(ints, RANDOM_SIZE,
			sizeof(int));

	cgs_array_sort(ai, cgs_int_cmp);

	int min = *(int*)cgs_array_get(ai, 0);
	int max = *(int*)cgs_array_get(ai, 9);
	assert_int_equal(min, -22);
	assert_int_equal(max, 37);

	cgs_array_free(ai);
}

static void array_find_test(void** state)
{
	const int* ints = *(const int**)state;

	struct cgs_array* ai = cgs_array_new_from_array(ints, RANDOM_SIZE,
			sizeof(int));

	int x = 99;
	int* found = cgs_array_find(ai, &x, cgs_int_cmp);
	assert_null(found);
	x = -13;
	found = cgs_array_find(ai, &x, cgs_int_cmp);
	assert_non_null(found);
	assert_ptr_not_equal(found, &x);
	assert_int_equal(*found, -13);

	cgs_array_free(ai);
}

static void array_xfer_test(void** state)
{
	const int* ints = *(const int**)state;

	struct cgs_array* ai = cgs_array_new_from_array(ints, RANDOM_SIZE,
			sizeof(int));

	size_t len = 0;
	int* out = cgs_array_xfer(ai, &len);

	assert_int_equal(len, 10);
	assert_non_null(out);
	assert_ptr_not_equal(out, ints);

	for (size_t i = 0; i < len; ++i)
		assert_int_equal(out[i], ints[i]);

	free(out);
}

static void array_new_from_array_test(void** state)
{
	const int* ints = *(const int**)state;

	struct cgs_array* ai = cgs_array_new_from_array(ints, RANDOM_SIZE,
			sizeof(int));

	assert_non_null(ai);
	assert_int_equal(cgs_array_length(ai), RANDOM_SIZE);

	for (size_t i = 0; i < cgs_array_length(ai); ++i)
		assert_int_equal(*(int*)cgs_array_get(ai, i), ints[i]);

	cgs_array_free(ai);
}

/*
int array_iter_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	const int* b = cgs_array_begin(ai);
	assert(b != NULL);

	const int* e = cgs_array_end(ai);
	assert(e != NULL);

	int sum = 0;
	for ( ; b != e; ++b)
		sum += *b;

	assert(sum == 55);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_strings_test(void* data)
{
	(void)data;

	const char* const strs[5] = {
		"Zora",
		"Anola",
		"Hazelridge",
		"Oakbank",
		"Dugald",
	};

	// New
	struct cgs_array* as = cgs_array_new(char*);
	// Push
	for (int i = 0; i < 5; ++i) {
		char* p = cgs_strdup(strs[i]);
		cgs_array_push(as, &p);
	}
	assert(cgs_array_length(as) == 5);

	// Get :(
	const char*const* p = cgs_array_get(as, 0);
	assert(strcmp(*p, strs[0]) == 0);

	// Iterators :)
	CgsStrIter b = cgs_array_begin(as);
	CgsStrIter e = cgs_array_end(as);
	for (CgsStrIter s = strs; b != e; ++b, ++s)
		assert(strcmp(*b, *s) == 0);

	// Free
	cgs_array_free_all(as);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		//{ "array_new", array_new_test, NULL },
		{ "array_push", array_push_test, test_int_10 },
		{ "array_sort", array_sort_test, test_int_10 },
		{ "array_find", array_find_test, test_int_10 },
		{ "array_xfer", array_xfer_test, test_int_10 },
		{ "array_new_from_array", array_new_from_array_test,
			test_int_10 },
		{ "array_strings", array_strings_test, NULL },
		{ "array_iter", array_iter_test, test_int_iota },
	};

	return cgs_run_tests(tests);
}
*/

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(array_new_test),
		cmocka_unit_test(array_push_test),
		cmocka_unit_test(array_sort_test),
		cmocka_unit_test(array_find_test),
		cmocka_unit_test(array_xfer_test),
		cmocka_unit_test(array_new_from_array_test),
	};

	return cmocka_run_group_tests(tests, setup_random, teardown_random);
}

