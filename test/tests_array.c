#include "cgs_array.h"
#include "cgs_test.h"

int test_int_10[10] = { 37, 2, 10, -13, -22, 5, 18, -1, 0, 30 };

int array_new_test(void* data)
{
	(void)data;

	struct cgs_array* ai = cgs_array_new(int);

	assert(ai != NULL);
	assert(cgs_array_length(ai) == 0);

	cgs_array_free(ai);

	struct cgs_array* ad = cgs_array_new(double);
	assert(ad != NULL);
	assert(cgs_array_length(ad) == 0);

	cgs_array_free(ad);

	struct cgs_array* as = cgs_array_new(char*);
	assert(as != NULL);
	assert(cgs_array_length(as) == 0);

	cgs_array_free(as);

	return TEST_SUCCESS;
}

int array_push_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new(int);
	for (int i = 0; i < 10; ++i)
		cgs_array_push(ai, &ints[i]);

	assert(cgs_array_length(ai) == 10);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_sort_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	cgs_array_sort(ai, cgs_int_cmp);

	int min = *(int*)cgs_array_get(ai, 0);
	int max = *(int*)cgs_array_get(ai, 9);
	assert(min == -22);
	assert(max == 37);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_find_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	int x = 99;
	int* found = cgs_array_find(ai, &x, cgs_int_cmp);
	assert(found == NULL);
	x = -13;
	found = cgs_array_find(ai, &x, cgs_int_cmp);
	assert(found != NULL);
	assert(found != &x);
	assert(*found == -13);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_xfer_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	size_t len = 0;
	int* out = cgs_array_xfer(ai, &len);

	assert(len == 10);
	assert(out != NULL);
	assert(out != ints);

	for (size_t i = 0; i < len; ++i)
		assert(out[i] == ints[i]);

	free(out);

	return TEST_SUCCESS;
}

int array_new_from_array_test(void* data)
{
	const int* ints = (const int*)data;

	struct cgs_array* ai = cgs_array_new_from_array(ints, 10, sizeof(int));

	assert(ai != NULL);
	assert(cgs_array_length(ai) == 10);

	for (size_t i = 0; i < cgs_array_length(ai); ++i)
		assert(*(int*)cgs_array_get(ai, i) == ints[i]);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "array_new", array_new_test, NULL },
		{ "array_push", array_push_test, test_int_10 },
		{ "array_sort", array_sort_test, test_int_10 },
		{ "array_find", array_find_test, test_int_10 },
		{ "array_xfer", array_xfer_test, test_int_10 },
		{ "array_new_from_array", array_new_from_array_test,
			test_int_10 },
	};

	return cgs_run_tests(tests);
}

