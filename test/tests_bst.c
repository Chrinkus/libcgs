#include "cgs_bst.h"
#include "cgs_variant.h"

#include "cgs_test.h"

const int garr[] = { 10, 14, 2, -13, 37, -6, 100, 0, -1, 8, -67, 72 };
const int glen = ARR_SIZE(garr);

int int_cmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int bst_int_new_test(void* data)
{
	(void)data;

	struct cgs_bst* tree = cgs_bst_new(int_cmp);
	assert(tree != NULL);
	assert(cgs_bst_size(tree) == 0);

	cgs_bst_free(tree);

	return TEST_SUCCESS;
}

int bst_int_insert_test(void* data)
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

int bst_int_search_test(void* data)
{
	(void)data;

	struct cgs_bst* tree = cgs_bst_new(int_cmp);
	struct cgs_variant var = { 0 };
	for (int i = 0; i < glen; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		cgs_bst_insert(tree, &var);
	}

	// re-usable pointers
	const struct cgs_variant* pv;
	const int* pi;

	for (int i = 0; i < glen; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		pv = cgs_bst_search(tree, &var);
		assert(pv != NULL);
		pi = cgs_variant_get(pv);
		assert(*pi == garr[i]);
	}

	// find fail
	cgs_variant_set_int(&var, 53);
	pv = cgs_bst_search(tree, &var);
	assert(pv == NULL);
	// insert it and find it
	cgs_bst_insert(tree, &var);
	pv = cgs_bst_search(tree, &var);
	assert(pv != NULL);
	pi = cgs_variant_get(pv);
	assert(*pi == 53);

	// and again with negative value
	cgs_variant_set_int(&var, -42);
	pv = cgs_bst_search(tree, &var);
	assert(pv == NULL);
	cgs_bst_insert(tree, &var);
	pv = cgs_bst_search(tree, &var);
	assert(pv != NULL);
	pi = cgs_variant_get(pv);
	assert(*pi == -42);

	cgs_bst_free(tree);

	return TEST_SUCCESS;
}

int bst_int_minmax_test(void* data)
{
	(void)data;

	struct cgs_bst* tree = cgs_bst_new(int_cmp);
	struct cgs_variant var = { 0 };

	int i = 0;
	for ( ; i < 3; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		cgs_bst_insert(tree, &var);
	}

	const int* min = cgs_bst_min(tree);
	const int* max = cgs_bst_max(tree);
	assert(*min == 2);
	assert(*max == 14);
	assert(cgs_bst_size(tree) == 3);

	for ( ; i < 10; ++i) {
		cgs_variant_set_int(&var, garr[i]);
		cgs_bst_insert(tree, &var);
	}

	min = cgs_bst_min(tree);
	max = cgs_bst_max(tree);
	assert(*min == -13);
	assert(*max == 100);
	assert(cgs_bst_size(tree) == 10);

	for ( ; i < glen; ++i) {
		cgs_variant_set_int(&var, garr[i]);
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
		{ "bst_int_new", bst_int_new_test, NULL },
		{ "bst_int_insert", bst_int_insert_test, NULL },
		{ "bst_int_minmax", bst_int_minmax_test, NULL },
		{ "bst_int_search", bst_int_search_test, NULL },
	};

	return cgs_run_tests(tests);
}

