#include "cgs_test.h"

#include "cgs_array.h"

CGS_ARRAY_DEFINE_STRUCT(int_array, int);

int array_init_free_test(void* data)
{
	(void)data;

	struct int_array arr;
	cgs_array_init(&arr);

	assert(arr.len == 0);
	assert(arr.mem == CGS_ARRAY_INITIAL_MEMORY);
	assert(arr.arr != NULL);

	cgs_array_free(&arr);

	return TEST_SUCCESS;
}

int array_push_test(void* data)
{
	(void)data;

	struct int_array arr;
	cgs_array_init(&arr);

	cgs_array_push(&arr, 2);
	cgs_array_push(&arr, 0);
	cgs_array_push(&arr, -6);
	cgs_array_push(&arr, 10);
	cgs_array_push(&arr, -5);

	assert(arr.len == 5);

	cgs_array_free(&arr);

	return TEST_SUCCESS;
}

int array_xfer_test(void* data)
{
	(void)data;

	struct int_array arr;
	cgs_array_init(&arr);

	cgs_array_push(&arr, -13);
	cgs_array_push(&arr, 31);
	cgs_array_push(&arr, 3);

	int* pi;
	cgs_array_xfer(&arr, pi);

	assert(pi[0] == -13);
	assert(pi[1] == 31);
	assert(pi[2] == 3);

	free(pi);

	return TEST_SUCCESS;
}

int array_get_test(void* data)
{
	(void)data;

	struct int_array arr;
	cgs_array_init(&arr);

	for (int i = 1; i <= 10; ++i)
		cgs_array_push(&arr, i);

	int sum = 0;
	for (size_t i = 0; i < arr.len; ++i)
		sum += cgs_array_get(&arr, i);

	assert(sum == 55);

	cgs_array_free(&arr);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "array_init_free", array_init_free_test, NULL },
		{ "array_push", array_push_test, NULL },
		{ "array_xfer", array_xfer_test, NULL },
		{ "array_get", array_get_test, NULL },
	};

	return cgs_run_tests(tests);
}
