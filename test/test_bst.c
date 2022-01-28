#include "cgs_bst.h"

#include "test_utils.h"

int bst_new_test(void* data)
{
	(void)data;

	struct cgs_bst* tree = cgs_bst_new();
	assert(tree != NULL);
	assert(cgs_bst_size(tree) == 0);

	cgs_bst_free(tree);

	return TEST_SUCCESS;
}

int bst_insert_test(void* data)
{
	(void)data;

	struct cgs_bst* tree = cgs_bst_new();
	assert(cgs_bst_insert(tree, 5));
	assert(cgs_bst_size(tree) == 1);

	cgs_bst_free(tree);

	return TEST_SUCCESS;
}

int bst_minmax_test(void* data)
{
	(void)data;

	struct cgs_bst* tree = cgs_bst_new();
	cgs_bst_insert(tree, 10);
	cgs_bst_insert(tree, 14);
	cgs_bst_insert(tree, 2);
	assert(cgs_bst_min(tree) == 2);
	assert(cgs_bst_max(tree) == 14);

	cgs_bst_insert(tree, -13);
	cgs_bst_insert(tree, 37);
	cgs_bst_insert(tree, -6);
	cgs_bst_insert(tree, 100);
	cgs_bst_insert(tree, 0);
	cgs_bst_insert(tree, -1);
	cgs_bst_insert(tree, 8);
	assert(cgs_bst_min(tree) == -13);
	assert(cgs_bst_max(tree) == 100);

	cgs_bst_insert(tree, -67);
	cgs_bst_insert(tree, 72);
	assert(cgs_bst_min(tree) == -67);
	assert(cgs_bst_max(tree) == 100);

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

