#include "cgs_array.h"
#include "cgs_test.h"

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
	(void)data;

	struct cgs_array* ai = cgs_array_new(int);

	int test[] = { 37, 2, 10, -13, -22, 5, 18, -1, 0, 30 };
	for (int i = 0; i < 10; ++i)
		cgs_array_push(ai, &test[i]);

	assert(cgs_array_length(ai) == 10);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int array_sort_test(void* data)
{
	(void)data;

	struct cgs_array* ai = cgs_array_new(int);
	int test[] = { 37, 2, 10, -13, -22, 5, 18, -1, 0, 30 };
	for (int i = 0; i < 10; ++i)
		cgs_array_push(ai, &test[i]);

	cgs_array_sort(ai, cgs_int_cmp);

	int min = *(int*)cgs_array_get(ai, 0);
	int max = *(int*)cgs_array_get(ai, 9);
	assert(min == -22);
	assert(max == 37);

	cgs_array_free(ai);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "array_new", array_new_test, NULL },
		{ "array_push", array_push_test, NULL },
		{ "array_sort", array_sort_test, NULL },
	};

	return cgs_run_tests(tests);
}

