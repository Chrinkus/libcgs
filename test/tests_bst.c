#include "cgs_bst.h"
#include "cgs_variant.h"

#include "test_utils.h"

int int_cmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int bst_new_test(void* data)
{
	(void)data;

	struct cgs_bst* tree = cgs_bst_new(int_cmp);
	assert(tree != NULL);
	assert(cgs_bst_size(tree) == 0);

	cgs_bst_free(tree);

	return TEST_SUCCESS;
}

int bst_insert_test(void* data)
{
	(void)data;

	struct cgs_bst* tree = cgs_bst_new(int_cmp);
	struct cgs_variant var = { 0 };

	cgs_variant_set_int(&var, 5);

	assert(cgs_bst_insert(tree, &var));
	assert(cgs_bst_size(tree) == 1);

	cgs_bst_free(tree);

	return TEST_SUCCESS;
}

int bst_minmax_test(void* data)
{
	(void)data;

	int arr[] = { 10, 14, 2, -13, 37, -6, 100, 0, -1, 8, -67, 72 };
	int len = ARR_SIZE(arr);

	struct cgs_bst* tree = cgs_bst_new(int_cmp);
	struct cgs_variant var = { 0 };

	int i = 0;
	for ( ; i < 3; ++i) {
		cgs_variant_set_int(&var, arr[i]);
		cgs_bst_insert(tree, &var);
	}

	const int* min = cgs_bst_min(tree);
	const int* max = cgs_bst_max(tree);
	assert(*min == 2);
	assert(*max == 14);
	assert(cgs_bst_size(tree) == 3);

	for ( ; i < 10; ++i) {
		cgs_variant_set_int(&var, arr[i]);
		cgs_bst_insert(tree, &var);
	}

	min = cgs_bst_min(tree);
	max = cgs_bst_max(tree);
	assert(*min == -13);
	assert(*max == 100);
	assert(cgs_bst_size(tree) == 10);

	for ( ; i < len; ++i) {
		cgs_variant_set_int(&var, arr[i]);
		cgs_bst_insert(tree, &var);
	}

	min = cgs_bst_min(tree);
	max = cgs_bst_max(tree);
	assert(*min == -67);
	assert(*max == 100);
	assert(cgs_bst_size(tree) == 12);

	cgs_bst_free(tree);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "bst_new", bst_new_test, NULL },
		{ "bst_insert", bst_insert_test, NULL },
		{ "bst_minmax", bst_minmax_test, NULL },
	};

	return run_tests(tests, ARR_SIZE(tests));
}

